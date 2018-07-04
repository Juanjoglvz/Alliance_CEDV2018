// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyController.h"
#include "MeleeController.generated.h"

/**
 * 
 */
UCLASS()
class ALLIANCE_API AMeleeController : public AEnemyController
{
	GENERATED_BODY()
	
public:
	AMeleeController();

	virtual void Possess(APawn* InPawn) override;
	
};