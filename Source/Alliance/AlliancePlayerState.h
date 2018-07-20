// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AlliancePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ALLIANCE_API AAlliancePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	
	AAlliancePlayerState();
	
	//void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;
	
};
