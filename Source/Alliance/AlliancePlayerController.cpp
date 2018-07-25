// Fill out your copyright notice in the Description page of Project Settings.

#include "AlliancePlayerController.h"
#include "AllianceGameMode.h"


void AAlliancePlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Executing BeginPlay on PlayerController"));

	OnServerAssignCharacter();
}

void AAlliancePlayerController::OnClientLogin_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Executing onclientlogin"));
	AAllianceGameMode* GameMode = Cast<AAllianceGameMode>(GetWorld()->GetAuthGameMode());
	if (!GIsServer) // Only respawn if not server (Client will be the second player)
	{
		GameMode->RespawnPlayer(this);
	}
		
}

FString AAlliancePlayerController::GetPlayerNameFromController() const
{
	FString Name = FString();
	Name = PlayerState->PlayerName;
	return Name;
}

void AAlliancePlayerController::OnServerAssignCharacter_Implementation()
{
	AGameModeBase* GMode = GetWorld()->GetAuthGameMode();
	AAllianceGameMode* Gamemode = Cast<AAllianceGameMode>(GMode);

	if (Gamemode)
	{
		Gamemode->RespawnPlayer(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Gamemode is null on PlayerController"));
	}
}
