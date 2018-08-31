/* Copyright (C) 2018 Iván García, Juan José Corroto and Javier Córdoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#include "AlliancePlayerController.h"
#include "AllianceGameMode.h"
#include "AllianceCharacter.h"
#include "DlgContext.h"
#include "DlgManager.h"


void AAlliancePlayerController::BeginPlay()
{
	Super::BeginPlay();

	OnServerAssignCharacter();
}

void AAlliancePlayerController::StartDialogue(class UDlgDialogue* Dialogue, UObject* FirstParticipant, UObject* SecondParticipant)
{
	ActiveContext = UDlgManager::StartDialogue2(Dialogue, FirstParticipant, SecondParticipant);
}

void AAlliancePlayerController::StartDialogue3Characters(class UDlgDialogue* Dialogue, UObject* FirstParticipant, UObject* SecondParticipant, UObject* ThirdParticipant)
{
	ActiveContext = UDlgManager::StartDialogue3(Dialogue, FirstParticipant, SecondParticipant, ThirdParticipant);
}

void AAlliancePlayerController::SelectDialogueOption(int32 Index)
{
	if (ActiveContext == nullptr || Index < 0 || Index >= ActiveContext->GetOptionNum())
		return;

	if (!ActiveContext->ChooseChild(Index))
		ActiveContext = nullptr;
}

void AAlliancePlayerController::OnClientLogin_Implementation()
{
	AAllianceGameMode* GameMode = Cast<AAllianceGameMode>(GetWorld()->GetAuthGameMode());
	if (!GIsServer) // Only respawn if not server (Client will be the second player)
	{
		GameMode->RespawnPlayer(this);
	}
		
}

FString AAlliancePlayerController::GetPlayerNameFromController() const
{
	FString Name = FString();
	Name = PlayerState->GetPlayerName();
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
