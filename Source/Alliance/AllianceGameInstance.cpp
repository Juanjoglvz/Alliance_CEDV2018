// Fill out your copyright notice in the Description page of Project Settings.

#include "AllianceGameInstance.h"
#include "UnrealNetwork.h"


UAllianceGameInstance::UAllianceGameInstance()
{

}

void UAllianceGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAllianceGameInstance, AssignedCharacters);
}
