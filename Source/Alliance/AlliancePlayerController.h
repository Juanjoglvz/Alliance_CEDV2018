// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/PlayerController.h"
#include "AlliancePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ALLIANCE_API AAlliancePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	UFUNCTION(Client, Reliable)
	void OnClientLogin();
	void OnClientLogin_Implementation();
	
	
};
