// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyController.h"
#include "EuronymousController.generated.h"

/**
 * 
 */
UCLASS()
class ALLIANCE_API AEuronymousController : public AEnemyController
{
	GENERATED_BODY()
	
	
public:
	AEuronymousController();

	virtual void Possess(APawn* InPawn) override;

	UFUNCTION()
		void SensePawn(const TArray<AActor*> &UpdatedActors);
	
};
