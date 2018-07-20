// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Engine/GameInstance.h"
#include "AllianceCharacter.h"
#include "AllianceGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALLIANCE_API UAllianceGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UAllianceGameInstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		TMap<FString, TSubclassOf<AAllianceCharacter>> AssignedCharacters;
	
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;
	
};
