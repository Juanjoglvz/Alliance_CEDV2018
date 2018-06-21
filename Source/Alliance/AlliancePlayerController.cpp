// Fill out your copyright notice in the Description page of Project Settings.

#include "AlliancePlayerController.h"
#include "AllianceGameMode.h"

void AAlliancePlayerController::OnClientLogin_Implementation()
{
	UE_LOG(LogTemp, Error, TEXT("Executing onclientlogin"));
	AAllianceGameMode* GameMode = Cast<AAllianceGameMode>(GetWorld()->GetAuthGameMode());
	if (!GIsServer) // Only respawn if not server (Client will be the second player)
	{
		GameMode->RespawnSecondPlayer(this);
	}
		
}
