/* Copyright (C) 2018 Ivan Garcia, Juan Jose Corroto and Javier Cordoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#pragma once

#include "CoreMinimal.h"
#include "AllianceController.h"
#include "Boss.h"
#include "EuronymousController.generated.h"

/**
 * 
 */
UCLASS()
class ALLIANCE_API AEuronymousController : public AAllianceController
{
	GENERATED_BODY()
	
	
public:
	AEuronymousController();

	UPROPERTY()
		ABoss* EuronymousReference;

	virtual void Possess(APawn* InPawn) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SensePawn(const TArray<AActor*> &UpdatedActors);

};
