/* Copyright (C) 2018 Iván García, Juan José Corroto and Javier Córdoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#include "AllianceCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "AlliancePlayerController.h"
#include "AllianceGameMode.h"
#include "Camera/CameraComponent.h"
#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"


AAllianceCharacter::AAllianceCharacter() : CurrentState{ EState::S_Idle }, b_ChainAttack{ false }, Sprint{ 1200.f }, LaunchForce{ 1.f }, 
LaunchHeight{ 1.f }, Combo{ 0 }, b_IsDead{ false }, InMinigame{ false }, b_IAmServer{ false }, DamageMultiplier{ 1.f }, DlgParticipantName { "AllianceCharacter" }
{
	// Set the actor to tick every frame
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	// Character replication stuff
	bReplicates = true;
	bReplicateMovement = true;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->SetNetAddressable(); // Make DSO components net addressable
	GetCharacterMovement()->SetIsReplicated(true); // Enable replication by default

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

}

void AAllianceCharacter::BeginPlay()
{
	Super::BeginPlay();	

	if (HasAuthority())
	{
		b_IAmServer = true;
	}
}

// Called every frame
void AAllianceCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

		switch (CurrentState)
		{
		case EState::S_Idle:
			if (Stamina < MaxStamina - DeltaTime * 10)
			{
				Stamina += DeltaTime * 10;
			}
			break;
		case EState::S_Running:
			if (Stamina > 0)
			{
				Stamina -= DeltaTime * 10;
			}
			else 
			{
				CurrentState = EState::S_Idle;
				ResetSpeed();
			}
			break;
		case EState::S_Blocking:
			if (Stamina < MaxStamina - DeltaTime * 4)
			{
				Stamina += DeltaTime * 4;
			}
			break;
		default:
			break;
		}

}

void AAllianceCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	CharacterMovementInputComponent = PlayerInputComponent;


	PlayerInputComponent->BindAxis("MoveForward", this, &AAllianceCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAllianceCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAllianceCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AAllianceCharacter::LookUpAtRate);

	// Bindings for actions
	PlayerInputComponent->BindAction("Primary_Attack", EInputEvent::IE_Pressed, this, &AAllianceCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AAllianceCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &AAllianceCharacter::StopSprint);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &AAllianceCharacter::Interact);
	PlayerInputComponent->BindAction("Block", EInputEvent::IE_Pressed, this, &AAllianceCharacter::StartBlock);
	PlayerInputComponent->BindAction("Block", EInputEvent::IE_Released, this, &AAllianceCharacter::StopBlock);
	InMinigame = false;
}

void AAllianceCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAllianceCharacter, Health);
	DOREPLIFETIME(AAllianceCharacter, Stamina);
	DOREPLIFETIME(AAllianceCharacter, b_IsDead);
	DOREPLIFETIME(AAllianceCharacter, b_ChainAttack);
	DOREPLIFETIME(AAllianceCharacter, LaunchForce);
	DOREPLIFETIME(AAllianceCharacter, LaunchHeight);
	DOREPLIFETIME(AAllianceCharacter, Combo);
}

void AAllianceCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAllianceCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAllianceCharacter::PrimaryAttack()
{
	if (HasAuthority())
	{
		OnAttackEvent.Broadcast();
	}
	else
	{
		// Client notifies server to attack
		OnServerClientAttacking();
	}
}

void AAllianceCharacter::StartSprint()
{
	if (HasAuthority())
	{
		StartSprinting();
	}
	else
	{
		// Client notifies server to start sprinting
		OnServerClientStartSprinting();
	}
}

void AAllianceCharacter::StopSprint()
{
	if (HasAuthority())
	{
		StopSprinting();
	}
	else
	{
		// Client notifies server to stop sprinting
		OnServerClientStopSprinting();
	}
}

void AAllianceCharacter::StartBlock()
{
	if (HasAuthority())
	{
		StartBlocking();
	}
	else
	{
		// Client notifies server to start blocking
		OnServerClientStartBlocking();
	}
}

void AAllianceCharacter::StopBlock()
{
	if (HasAuthority())
	{
		StopBlocking();
	}
	else
	{
		// Client notifies server to stop blocking
		OnServerClientStopBlocking();
	}
}

void AAllianceCharacter::ResetMoveSpeed()
{
	if (HasAuthority())
	{
		ResetSpeed();
	}
	else
	{
		// Client notifies server to reset the sprint speed
		OnServerClientResetSpeed();
	}
}

void AAllianceCharacter::Interact()
{
	if (HasAuthority())
	{
		b_IAmServer = true;
	}
	OnStartMinigame.Broadcast();
}

EState AAllianceCharacter::GetState() const
{
	if (GetCharacterMovement()->MaxWalkSpeed == 600.f)
	{
		return EState::S_Blocking;
	}
	else if (GetCharacterMovement()->MaxWalkSpeed == 1200.f + Sprint)
	{
		return EState::S_Running;
	}
	else
	{
		return EState::S_Idle;
	}
}

void AAllianceCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && CurrentState != EState::S_Attacking 
		&& CurrentState != EState::S_JumpAttacking && CurrentState != EState::S_SpecialAttacking)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAllianceCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && CurrentState != EState::S_Attacking
		&& CurrentState != EState::S_JumpAttacking && CurrentState != EState::S_SpecialAttacking)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

float AAllianceCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.f && !b_IsEvading)
	{
		ActualDamage *= DamageMultiplier;
		Health -= ActualDamage;
		if (Health <= 0)
		{
			// The character is dead
			Health = 0.f;
			ExecuteWhenDead();
		}

		if (Health <= 50)
		{
			Stamina -= 5;
		}
	}
	return ActualDamage;
}

void AAllianceCharacter::DoDmg(AActor* DamagedActor, float Dmg) const
{
	UGameplayStatics::ApplyDamage(DamagedActor, Dmg, nullptr, nullptr, nullptr);
}

void AAllianceCharacter::ExecuteWhenDead_Implementation()
{
	this->Destroy();
	b_IsDead = true;
}

void AAllianceCharacter::OnServerClientAttacking_Implementation()
{
	if (HasAuthority())
	{
		OnAttackEvent.Broadcast();
	}
}

void AAllianceCharacter::StartSprinting_Implementation()
{
	if (!InMinigame)
	{
		CurrentState = EState::S_Running;
		ResetMoveSpeed();
	}
}

void AAllianceCharacter::StopSprinting_Implementation()
{
	if (!InMinigame)
	{
		CurrentState = EState::S_Idle;
		ResetMoveSpeed();
	}
}

void AAllianceCharacter::StartBlocking_Implementation()
{
	if (!InMinigame)
	{
		CurrentState = EState::S_Blocking;
		ResetMoveSpeed();
		DamageMultiplier = 0.4f;
	}
}

void AAllianceCharacter::StopBlocking_Implementation()
{
	if (!InMinigame)
	{
		CurrentState = EState::S_Idle;
		ResetMoveSpeed();
		DamageMultiplier = 1.f;
	}
}

void AAllianceCharacter::ResetSpeed_Implementation()
{
	if (!InMinigame)
	{
		switch (CurrentState)
		{
		case EState::S_Idle:
			GetCharacterMovement()->MaxWalkSpeed = 1200.f;
			break;
		case EState::S_Running:
			GetCharacterMovement()->MaxWalkSpeed = 1200.f + Sprint;
			break;
		case EState::S_Blocking:
			GetCharacterMovement()->MaxWalkSpeed = 600.f;
			break;
		default:
			GetCharacterMovement()->MaxWalkSpeed = 1200.f;
			break;
		}
	}
}

void AAllianceCharacter::OnServerClientStartSprinting_Implementation()
{
	if (HasAuthority())
	{
		StartSprinting();
	}
}

void AAllianceCharacter::OnServerClientStopSprinting_Implementation()
{
	if (HasAuthority())
	{
		StopSprinting();
	}
}

void AAllianceCharacter::OnServerClientStartBlocking_Implementation()
{
	if (HasAuthority())
	{
		StartBlocking();
	}
}

void AAllianceCharacter::OnServerClientStopBlocking_Implementation()
{
	if (HasAuthority())
	{
		StopBlocking();
	}
}

void AAllianceCharacter::OnServerClientResetSpeed_Implementation()
{
	if (HasAuthority())
	{
		ResetSpeed();
	}
}

void AAllianceCharacter::OnServerStartMinigame_Implementation()
{
	if (HasAuthority())
	{
		OnStartMinigame.Broadcast();
	}
}

void AAllianceCharacter::OnServerAssignCharacter_Implementation()
{
	if (GIsServer) // Should always be true
	{
		AController* Controller = GetController();
		AAlliancePlayerController* PlayerController = Cast<AAlliancePlayerController>(Controller);

		AGameModeBase* GMode = GetWorld()->GetAuthGameMode();
		AAllianceGameMode* Gamemode = Cast<AAllianceGameMode>(GMode);

		if (PlayerController && Gamemode)
		{
			Gamemode->RespawnPlayer(PlayerController);
		}
	}
}

void AAllianceCharacter::SetCharacterMovement(class UInputComponent* InputComponent)
{
	SetupPlayerInputComponent(InputComponent);
}

void AAllianceCharacter::RemoveCharacterMovementBindings()
{
	InMinigame = true;
	// Remove axis bindings
	while (CharacterMovementInputComponent->AxisBindings.Num() > 0)
	{
		CharacterMovementInputComponent->AxisBindings.RemoveAt(0);
	}
}

// Override dialogue interface functions

bool AAllianceCharacter::ModifyIntValue_Implementation(const FName& ValueName, bool bDelta, int32 Value)
{
	if (!DlgData.Integers.Contains(ValueName))
		DlgData.Integers.Add(ValueName, 0);

	if (bDelta)
		DlgData.Integers[ValueName] += Value;
	else
		DlgData.Integers[ValueName] = Value;

	return true;
}

bool AAllianceCharacter::ModifyFloatValue_Implementation(const FName& ValueName, bool bDelta, float Value)
{
	if (!DlgData.Floats.Contains(ValueName))
		DlgData.Floats.Add(ValueName, 0.0f);

	if (bDelta)
		DlgData.Floats[ValueName] += Value;
	else
		DlgData.Floats[ValueName] = Value;

	return true;
}

bool AAllianceCharacter::ModifyBoolValue_Implementation(const FName& ValueName, bool bValue)
{
	if (bValue)
		DlgData.TrueBools.Add(ValueName);
	else
		DlgData.TrueBools.Remove(ValueName);

	return true;
}

bool AAllianceCharacter::ModifyNameValue_Implementation(const FName& ValueName, const FName& NameValue)
{
	if (DlgData.Names.Contains(ValueName))
		DlgData.Names[ValueName] = NameValue;
	else
		DlgData.Names.Add(ValueName, NameValue);

	return true;
}


float AAllianceCharacter::GetFloatValue_Implementation(const FName& ValueName) const
{
	return DlgData.Floats.Contains(ValueName) ? DlgData.Floats[ValueName] : 0.0f;
}

int32 AAllianceCharacter::GetIntValue_Implementation(const FName& ValueName) const
{
	return DlgData.Integers.Contains(ValueName) ? DlgData.Integers[ValueName] : 0;
}

bool AAllianceCharacter::GetBoolValue_Implementation(const FName& ValueName) const
{
	return DlgData.TrueBools.Contains(ValueName);
}

FName AAllianceCharacter::GetNameValue_Implementation(const FName& ValueName) const
{
	return DlgData.Names.Contains(ValueName) ? DlgData.Names[ValueName] : NAME_None;
}