/* Copyright (C) 2018 Iván García, Juan José Corroto and Javier Córdoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AIController.h"
#include "AllianceController.generated.h"

/**
 * 
 */
UCLASS()
class ALLIANCE_API AAllianceController : public AAIController
{
	GENERATED_BODY()

public:

	AAllianceController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when a Pawn is possessed
	virtual void Possess(APawn* InPawn) override;
	
protected:
	/*A Behavior tree component in order to be able to call specific functions like starting our BT*/
	UBehaviorTreeComponent* BehaviorTreeComp;

	/*A Blackboard component which will be used to initialize our Blackboard Values*/
	UBlackboardComponent* BlackboardComp;

	// Component for sensing the players
	UAIPerceptionComponent* PerceptionComponent;
	// Configuration for the sight of the controller
	UAISenseConfig_Sight* SightConfig;



public:
	UPROPERTY(EditAnywhere)
		UBehaviorTree* BehaviourTree;
};
