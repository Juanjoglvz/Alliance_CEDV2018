/* Copyright (C) 2018 Ivan Garcia, Juan Jose Corroto and Javier Cordoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#include "ShivaController.h"
#include "AllianceCharacter.h"
#include "Enemy.h"

AShivaController::AShivaController()
{
	auto TreeAsset = ConstructorHelpers::FObjectFinder<UBehaviorTree>(TEXT("BehaviorTree'/Game/ThirdPersonCPP/Blueprints/Artificial_Intelligence/Shiva/Shiva_BT.Shiva_BT'"));
	BehaviourTree = TreeAsset.Object;
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AShivaController::SensePawn);
}

void AShivaController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, SightConfig->GetSenseImplementation(), InPawn);

	ShyvaReference = Cast<ABoss>(InPawn);

	AEnemy* Enemy = Cast<AEnemy>(InPawn);

	if (Enemy)
	{

		BlackboardComp->InitializeBlackboard(*(BehaviourTree->BlackboardAsset));

		BehaviorTreeComp->StartTree(*BehaviourTree);

		BlackboardComp->SetValueAsBool(FName{ "GetAggro" }, true);
		BlackboardComp->SetValueAsBool(FName{ "CanPowerUp" }, true);
		BlackboardComp->SetValueAsFloat(FName{ "AttackRange" }, 150);
		BlackboardComp->SetValueAsFloat(FName{ "PowerUpPerc" }, 0.3);

	}
}

void AShivaController::Tick(float DeltaTime)
{
	if (ShyvaReference)
	{
		BlackboardComp->SetValueAsFloat(FName{ "Hp" }, ShyvaReference->Health);

		if (ShyvaReference->Health < 300)
		{
			BlackboardComp->SetValueAsFloat(FName{ "PowerUpPerc" }, 0.5);
		}
	}
}

void AShivaController::SensePawn(const TArray<AActor*> &UpdatedActors)
{
	for (auto& Actor : UpdatedActors)
	{
		AAllianceCharacter* Player = Cast<AAllianceCharacter>(Actor);

		if (Player != nullptr)
		{
			if (BlackboardComp->GetValueAsBool(FName{ "GetAggro" }))
			{
				FName Key = FName{ "PlayerAggro" };
				BlackboardComp->SetValueAsObject(Key, Player);
				BlackboardComp->SetValueAsBool(FName{ "GetAggro" }, false);
			}
			break;
		}
	}
}


