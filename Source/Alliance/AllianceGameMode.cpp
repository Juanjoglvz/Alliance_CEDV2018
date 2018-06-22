// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AllianceGameMode.h"
#include "AllianceCharacter.h"
#include "AlliancePlayerController.h"
#include "UObject/ConstructorHelpers.h"

AAllianceGameMode::AAllianceGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//PlayerControllerClass = AAlliancePlayerController::StaticClass();
}

void AAllianceGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (GIsServer)
	{
		UE_LOG(LogTemp, Error, TEXT("Server: Post Login   Controller: %p"), NewPlayer);
		if (NewPlayer)
		{
			AAlliancePlayerController* PController = Cast<AAlliancePlayerController>(NewPlayer);

			if (PController)
			{
				PController->OnClientLogin();
				UE_LOG(LogTemp, Error, TEXT("Called onclientlogin from game mode"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Client: Post Login"));
	}
}

void AAllianceGameMode::RespawnSecondPlayer_Implementation(APlayerController * SecondPlayer)
{
	UE_LOG(LogTemp, Error, TEXT("Respawnsecondplayer called"));
	if (SecondPlayer->GetPawn()) // Player is in the world
		SecondPlayer->GetPawn()->Destroy();

	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAllianceCharacter::StaticClass(), FoundCharacters);

	if (FoundCharacters.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Guat"));
	}
	else if (FoundCharacters.Num() == 1)
	{
		AAllianceCharacter* MainCharacter = Cast<AAllianceCharacter>(FoundCharacters[0]);

		FVector NewPosition = MainCharacter->GetActorLocation();
		NewPosition.Y -= 1000;
		NewPosition.X -= 1000;

		FActorSpawnParameters SpawnInfo; 
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AAllianceCharacter* SpawnedCharacter = GetWorld()->SpawnActor<AAllianceCharacter>(NewPosition, MainCharacter->GetActorRotation(), SpawnInfo);

		SecondPlayer->Possess(SpawnedCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Qicess"));
	}
}
