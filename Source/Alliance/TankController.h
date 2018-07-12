// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyController.h"
#include "TankController.generated.h"

/**
 * 
 */
UCLASS()
class ALLIANCE_API ATankController : public AEnemyController
{
	GENERATED_BODY()
	
public:
	ATankController();

	virtual void Possess(APawn* InPawn) override;
	
	
};
