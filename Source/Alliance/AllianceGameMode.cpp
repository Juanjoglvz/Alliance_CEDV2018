/* Copyright (C) 2018 Ivan Garcia, Juan Jose Corroto and Javier Cordoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#include "AllianceGameMode.h"
#include "AllianceCharacter.h"
#include "AlliancePlayerController.h"
#include "AlliancePlayerState.h"
#include "AllianceGameInstance.h"
#include "AlyssaController.h"
#include "MortenController.h"
#include "UObject/ConstructorHelpers.h"

AAllianceGameMode::AAllianceGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/Characters_and_NPCs/Alyssa"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = AAlliancePlayerController::StaticClass();
	PlayerStateClass = AAlliancePlayerState::StaticClass();


	static ConstructorHelpers::FObjectFinder<UBlueprint> CharBlueprint(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/Characters_and_NPCs/Alyssa.Alyssa'"));
	if (CharBlueprint.Object) {
		FirstCharacter = (UClass*)CharBlueprint.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> CharBlueprint2(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/Characters_and_NPCs/Morten.Morten'"));
	if (CharBlueprint2.Object) {
		SecondCharacter = (UClass*)CharBlueprint2.Object->GeneratedClass;
	}

	FirstCharacterController = (UClass*)AAlyssaController::StaticClass();
	SecondCharacterController = (UClass*)AMortenController::StaticClass();

	CharacterControllers.Add(FirstCharacter, FirstCharacterController);
	CharacterControllers.Add(SecondCharacter, SecondCharacterController);
}

void AAllianceGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AAllianceGameMode::RespawnPlayer_Implementation(APlayerController * SecondPlayer)
{
	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if (LevelName.Contains("Main") || LevelName.Contains("Menu"))
		return;

	AAlliancePlayerController* PController = Cast<AAlliancePlayerController>(SecondPlayer);
	FString Name = PController->GetPlayerNameFromController(); // Unique player name
	if (Name.Len() == 0)
	{
		if (PController->GetPawn()) // Player is in the world
		{
			SecondPlayer->GetPawn()->Destroy();
		}
		return;
	}

	
	UAllianceGameInstance* GInstance;
	GInstance = Cast<UAllianceGameInstance>(GetGameInstance());

	if (!GInstance->AssignedCharacters.Contains(Name)) // Joining player has not been assigned a character
	{
		if (GInstance->AssignedCharacters.Num() == 1) // Joining player should have the free character
		{
			TArray<FString> Keys;
			GInstance->AssignedCharacters.GetKeys(Keys);
			FString MainPlayerName = Keys[0];

			if (GInstance->AssignedCharacters[MainPlayerName].Get()->IsChildOf(FirstCharacter.Get()))
				GInstance->AssignedCharacters.Add(Name, SecondCharacter);
			else
				GInstance->AssignedCharacters.Add(Name, FirstCharacter);
		}
		else // Joining player has not been assigned a character (PIE, Alyssa first character by default)
		{
			GInstance->AssignedCharacters.Add(Name, FirstCharacter);
		}
	}

	TSubclassOf<AAllianceCharacter> AssignedCharacter;
	TSubclassOf<AAllianceCharacter> SecondAssignedCharacter;

	AssignedCharacter = GInstance->AssignedCharacters[Name];

	if (AssignedCharacter.Get()->IsChildOf(FirstCharacter.Get())) // Get the character that the AI should possess
		SecondAssignedCharacter = SecondCharacter;
	else
		SecondAssignedCharacter = FirstCharacter;

	if (AssignedCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldnt retrieve the assigned character"));
	}

	if (SecondPlayer->GetPawn()) // Player is in the world
	{
		SecondPlayer->GetPawn()->Destroy();
	}

	TArray<AActor*> FoundCharacters; // Only one character should exist
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAllianceCharacter::StaticClass(), FoundCharacters); 

	if (FoundCharacters.Num() == 0) // First character in the world, use playerstart then spawn AI Character
	{
		TArray<AActor*> FoundPStarts;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundPStarts);

		if (FoundPStarts.Num() == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("No playerstarts found"));
			return;
		}

		APlayerStart* PStart = Cast<APlayerStart>(FoundPStarts[0]);

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AAllianceCharacter* SpawnedCharacter = GetWorld()->SpawnActor<AAllianceCharacter>(AssignedCharacter, PStart->GetActorLocation() , PStart->GetActorRotation(), SpawnInfo);

		SecondPlayer->Possess(SpawnedCharacter);

		// Spawn the second player, this time controlled by AI

		AAllianceCharacter* AIControlledCharacter = SpawnSecondPlayerNearFirst(SpawnedCharacter->GetActorLocation(), SpawnedCharacter->GetActorRotation(), SecondAssignedCharacter);

		if (AIControlledCharacter == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Second player controlled by AI couldnt be spawned"));
			return;
		}

		UClass* AIPlayerControllerClass = CharacterControllers[SecondAssignedCharacter];
		AAllianceController* AIPlayerController = GetWorld()->SpawnActor<AAllianceController>(AIPlayerControllerClass);

		if (!AIPlayerController)
		{
			UE_LOG(LogTemp, Error, TEXT("Couldnt spawn AI Player Controller"));
			return;
		}

		AIPlayerController->Possess(AIControlledCharacter);

	}
	else if (FoundCharacters.Num() == 1) // First character spawned, AI Character not spawned yet
	{
		AAllianceCharacter* MainCharacter = Cast<AAllianceCharacter>(FoundCharacters[0]);

		FVector NewPosition = MainCharacter->GetActorLocation();
		NewPosition.Y += 500;
		//NewPosition.X -= 1000;

		FActorSpawnParameters SpawnInfo; 
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AAllianceCharacter* SpawnedCharacter = GetWorld()->SpawnActor<AAllianceCharacter>(AssignedCharacter, NewPosition, MainCharacter->GetActorRotation(), SpawnInfo);

		SecondPlayer->Possess(SpawnedCharacter);
	}
	else if (FoundCharacters.Num() == 2) // Both characters spawned, just change the controller
	{
		AAllianceCharacter* SecondaryCharacter = GetControlledPawnByAI(FoundCharacters);

		SecondPlayer->Possess(SecondaryCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("A lot of characters found"));
	}
}

AAllianceCharacter * AAllianceGameMode::SpawnSecondPlayerNearFirst(FVector FirstPosition, FRotator FirstRotation, TSubclassOf<class AAllianceCharacter> SecondCharacter)
{
	FVector NewPosition = FirstPosition;
	NewPosition.Y += 500;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return GetWorld()->SpawnActor<AAllianceCharacter>(SecondCharacter, NewPosition, FirstRotation, SpawnInfo);
}

AAllianceCharacter* AAllianceGameMode::GetControlledPawnByAI(TArray<AActor*> CharactersInWorld)
{
	for (AActor* Character : CharactersInWorld)
	{
		AAllianceCharacter* ACharacter = Cast<AAllianceCharacter>(Character);
		
		if (ACharacter)
		{
			AController* Controller = ACharacter->GetController();

			AAllianceController* TentativeController = Cast<AAllianceController>(Controller);

			if (TentativeController)
				return ACharacter;
		}
	}
	return nullptr;
}
