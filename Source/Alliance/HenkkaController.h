/* Copyright (C) 2018 Iván García, Juan José Corroto and Javier Córdoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#pragma once

#include "CoreMinimal.h"
#include "AllianceController.h"
#include "HenkkaController.generated.h"

/**
 * 
 */
UCLASS()
class ALLIANCE_API AHenkkaController : public AAllianceController
{
	GENERATED_BODY()
	
public:
	AHenkkaController();

	virtual void Possess(APawn* InPawn) override;

	UFUNCTION()
		void SensePawn(const TArray<AActor*> &UpdatedActors);
	
	
};
