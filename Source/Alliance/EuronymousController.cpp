/* Copyright (C) 2018 Ivan Garcia, Juan Jose Corroto and Javier Cordoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#include "EuronymousController.h"
#include "AllianceCharacter.h"
#include "Enemy.h"

AEuronymousController::AEuronymousController() : EuronymousReference(nullptr)
{
	auto TreeAsset = ConstructorHelpers::FObjectFinder<UBehaviorTree>(TEXT("BehaviorTree'/Game/ThirdPersonCPP/Blueprints/Artificial_Intelligence/Euronymous/Euronymous_BT.Euronymous_BT'"));
	BehaviourTree = TreeAsset.Object;
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEuronymousController::SensePawn);
}

void AEuronymousController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, SightConfig->GetSenseImplementation(), InPawn);

	EuronymousReference = Cast<ABoss>(InPawn);

	if (EuronymousReference)
	{

		BlackboardComp->InitializeBlackboard(*(BehaviourTree->BlackboardAsset));

		BehaviorTreeComp->StartTree(*BehaviourTree);

		BlackboardComp->SetValueAsBool(FName{ "GetAggro" }, true);
		BlackboardComp->SetValueAsBool(FName{ "CanPwUp" }, true);
		BlackboardComp->SetValueAsFloat(FName{ "AttackRange" }, 150);
		BlackboardComp->SetValueAsFloat(FName{ "PowerUpPerc" }, 0.2);

	}
}

void AEuronymousController::Tick(float DeltaTime)
{
	if (EuronymousReference) 
	{
		BlackboardComp->SetValueAsFloat(FName{ "HP" }, EuronymousReference->Health);

		if (EuronymousReference->Health < 200)
		{
			BlackboardComp->SetValueAsFloat(FName{ "PowerUpPerc" }, 0.6);
		}
	}
}

void AEuronymousController::SensePawn(const TArray<AActor*> &UpdatedActors)
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