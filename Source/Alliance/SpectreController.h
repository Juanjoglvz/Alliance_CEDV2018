// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyController.h"
#include "SpectreController.generated.h"

/**
 * 
 */
UCLASS()
class ALLIANCE_API ASpectreController : public AEnemyController
{
	GENERATED_BODY()
	
public:
	ASpectreController();

	virtual void Possess(APawn* InPawn) override;

	UFUNCTION()
		void SensePawn(const TArray<AActor*> &UpdatedActors);
<<<<<<< HEAD

=======
	
	
>>>>>>> AI
};
