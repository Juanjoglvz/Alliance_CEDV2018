// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AllianceCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "AlliancePlayerController.h"
#include "Camera/CameraComponent.h"
#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"


AAllianceCharacter::AAllianceCharacter() : CurrentState{ EState::S_Idle }, b_ChainAttack{ false }, Sprint{ 1200.f }, LaunchForce{ 1.f },
LaunchHeight{ 1.f }, Combo{ 0 }, b_IsDead{ false }, InMinigame{ false }, b_IAmServer{ false }, DamageMultiplier{ 1.f }
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

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
	/*UE_LOG(LogTemp, Error, TEXT("Called begin play"));
	AController* Controller = GetController();
	AAlliancePlayerController* PlayerController = Cast<AAlliancePlayerController>(Controller);

	UE_LOG(LogTemp, Error, TEXT("Obtained controller: %p"), Controller);
	if (PlayerController)
		PlayerController->OnClientLogin();*/
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
			Stamina -= 15;
		}

		UE_LOG(LogTemp, Error, TEXT("Taken %f damage. Remaining life: %f"), ActualDamage, Health);
	}
	return ActualDamage;
}

void AAllianceCharacter::DoDmg(AActor* DamagedActor, float Dmg) const
{
	UGameplayStatics::ApplyDamage(DamagedActor, Dmg, nullptr, nullptr, nullptr);
}

void AAllianceCharacter::ExecuteWhenDead_Implementation()
{
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