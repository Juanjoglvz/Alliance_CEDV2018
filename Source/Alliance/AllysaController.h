// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyController.h"
#include "AllysaController.generated.h"

/**
 * 
 */
UCLASS()
class ALLIANCE_API AAllysaController : public AEnemyController
{
	GENERATED_BODY()
	
public:
	AAllysaController();

	virtual void Possess(APawn* InPawn) override;

	UFUNCTION()
		void SensePawn(const TArray<AActor*> &UpdatedActors);
	
	
};
