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

	PlayerControllerClass = AAlliancePlayerController::StaticClass();

	static ConstructorHelpers::FObjectFinder<UBlueprint> CharBlueprint(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter2.ThirdPersonCharacter2'"));
	if (CharBlueprint.Object) {
		FirstCharacter = (UClass*)CharBlueprint.Object->GeneratedClass;
	}
}

void AAllianceGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (GIsServer)
	{
		UE_LOG(LogTemp, Error, TEXT("Server: Post Login   Controller: %p"), NewPlayer);
		if (NewPlayer)
		{
			/*AAlliancePlayerController* PController = Cast<AAlliancePlayerController>(NewPlayer);

			if (PController)
			{
				PController->OnClientLogin();
				UE_LOG(LogTemp, Error, TEXT("Called onclientlogin from game mode"));
			}*/
			RespawnSecondPlayer_Implementation(NewPlayer);
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

	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAllianceCharacter::StaticClass(), FoundCharacters);
	
	if (FoundCharacters.Num() <= 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Not enough player characters"));
		return;
	}

	if (SecondPlayer->GetPawn()) // Player is in the world
	{
		UE_LOG(LogTemp, Error, TEXT("Second Player Destroyed: %s"), *SecondPlayer->GetPawn()->GetName());
		SecondPlayer->GetPawn()->Destroy();
	}

	FoundCharacters.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAllianceCharacter::StaticClass(), FoundCharacters);

	if (FoundCharacters.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No found characters"));
	}
	else if (FoundCharacters.Num() == 1 && FirstCharacter)
	{
		AAllianceCharacter* MainCharacter = Cast<AAllianceCharacter>(FoundCharacters[0]);

		FVector NewPosition = MainCharacter->GetActorLocation();
		NewPosition.Y += 500;
		//NewPosition.X -= 1000;

		FActorSpawnParameters SpawnInfo; 
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AAllianceCharacter* SpawnedCharacter = GetWorld()->SpawnActor<AAllianceCharacter>(FirstCharacter, NewPosition, MainCharacter->GetActorRotation(), SpawnInfo);

		SecondPlayer->Possess(SpawnedCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("A lot of characters found"));
	}
}
