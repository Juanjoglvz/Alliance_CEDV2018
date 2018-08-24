// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AllianceController.h"
#include "RangedController.generated.h"

/**
 * 
 */
UCLASS()
class ALLIANCE_API ARangedController : public AAllianceController
{
	GENERATED_BODY()
	
public:
	ARangedController();

	virtual void Possess(APawn* InPawn) override;

	UFUNCTION()
		void SensePawn(const TArray<AActor*> &UpdatedActors);
};
