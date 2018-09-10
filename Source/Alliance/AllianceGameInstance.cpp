/* Copyright (C) 2018 Ivan Garcia, Juan Jose Corroto and Javier Cordoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#include "AllianceGameInstance.h"
#include "UnrealNetwork.h"
#include "Online.h"


UAllianceGameInstance::UAllianceGameInstance()
{

}

void UAllianceGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAllianceGameInstance, AssignedCharacters);
}

bool UAllianceGameInstance::isSteamActivated(APlayerController* PlayerController)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get(FName("Steam"));

	if (OnlineSubsystem == nullptr)
	{
		return false;
	}
	
	ELoginStatus::Type onlineStatus = OnlineSubsystem->GetIdentityInterface()->GetLoginStatus(PlayerController->GetLocalPlayer()->GetControllerId());
	
	if (onlineStatus == ELoginStatus::Type::LoggedIn)
	{
		return true;
	}
	else
	{
		return false;
	}
}