// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AllianceGameMode.h"
#include "AllianceCharacter.h"
#include "AlliancePlayerController.h"
#include "AlliancePlayerState.h"
#include "AllianceGameInstance.h"
#include "UObject/ConstructorHelpers.h"

AAllianceGameMode::AAllianceGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/Alyssa"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = AAlliancePlayerController::StaticClass();
	PlayerStateClass = AAlliancePlayerState::StaticClass();


	static ConstructorHelpers::FObjectFinder<UBlueprint> CharBlueprint(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/Alyssa.Alyssa'"));
	if (CharBlueprint.Object) {
		FirstCharacter = (UClass*)CharBlueprint.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> CharBlueprint2(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/Morten.Morten'"));
	if (CharBlueprint2.Object) {
		SecondCharacter = (UClass*)CharBlueprint2.Object->GeneratedClass;
	}
}

void AAllianceGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	/*AAlliancePlayerController* PController = Cast<AAlliancePlayerController>(NewPlayer);
	if (PController)
		PController->OnServerAssignCharacter();*/
	/*if (GIsServer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Server: Post Login   Controller: %p"), NewPlayer);
		if (NewPlayer)
		{
			//AAlliancePlayerController* PController = Cast<AAlliancePlayerController>(NewPlayer);

			//if (PController)
			//{
			//	PController->OnClientLogin();
			//	UE_LOG(LogTemp, Error, TEXT("Called onclientlogin from game mode"));
			//}
			RespawnPlayer_Implementation(NewPlayer);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Client: Post Login"));
	}*/
}

void AAllianceGameMode::RespawnPlayer_Implementation(APlayerController * SecondPlayer)
{
	UE_LOG(LogTemp, Error, TEXT("RespawnPlayer called"));

	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if (LevelName.Contains("Main") || LevelName.Contains("Menu"))
		return;

	AAlliancePlayerController* PController = Cast<AAlliancePlayerController>(SecondPlayer);
	FString Name = PController->GetPlayerNameFromController(); // Unique player name
	if (Name.Len() == 0)
		return;

	
	UAllianceGameInstance* GInstance;
	GInstance = Cast<UAllianceGameInstance>(GetGameInstance());

	if (!GInstance->AssignedCharacters.Contains(Name))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player has not been assigned a character"));
		if (GInstance->AssignedCharacters.Num() == 1) // Joining player should have the other character
		{
			TArray<FString> Keys;
			GInstance->AssignedCharacters.GetKeys(Keys);
			FString MainPlayerName = Keys[0];

			if (GInstance->AssignedCharacters[MainPlayerName].Get()->IsChildOf(FirstCharacter.Get()))
			{
				GInstance->AssignedCharacters.Add(Name, SecondCharacter);
			}
			else
			{
				GInstance->AssignedCharacters.Add(Name, FirstCharacter);
			}
		}
		else // Joining player has not been assigned a character (PIE, Alyssa first character by default)
		{
			GInstance->AssignedCharacters.Add(Name, FirstCharacter);
		}

	}

	TSubclassOf<AAllianceCharacter> AssignedCharacter = GInstance->AssignedCharacters[Name];

	if (SecondPlayer->GetPawn()) // Player is in the world
	{
		UE_LOG(LogTemp, Error, TEXT("Second Player Destroyed: %s"), *SecondPlayer->GetPawn()->GetName());
		SecondPlayer->GetPawn()->Destroy();
	}

	TArray<AActor*> FoundCharacters; // Only one character should exist
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAllianceCharacter::StaticClass(), FoundCharacters); 

	if (FoundCharacters.Num() == 0) // First character in the world, use playerstart instead
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
	}
	else if (FoundCharacters.Num() == 1)
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
	else
	{
		UE_LOG(LogTemp, Error, TEXT("A lot of characters found"));
	}
}
