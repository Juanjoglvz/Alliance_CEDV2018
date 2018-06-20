// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AllianceGameMode.h"
#include "AllianceCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAllianceGameMode::AAllianceGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AAllianceGameMode::PostLogin(APlayerController * NewPlayer)
{
	if (GIsServer)
	{
		UE_LOG(LogTemp, Error, TEXT("Server: Post Login"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Client: Post Login"));
	}
}
