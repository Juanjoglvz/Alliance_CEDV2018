// Fill out your copyright notice in the Description page of Project Settings.

#include "EuronymousController.h"
#include "AllianceCharacter.h"
#include "Enemy.h"

AEuronymousController::AEuronymousController()
{
	auto TreeAsset = ConstructorHelpers::FObjectFinder<UBehaviorTree>(TEXT("BehaviorTree'/Game/ThirdPersonCPP/Blueprints/Artificial_Intelligence/Euronymous/Euronymous_BT.Euronymous_BT'"));
	BehaviourTree = TreeAsset.Object;
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEuronymousController::SensePawn);
}

void AEuronymousController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, SightConfig->GetSenseImplementation(), InPawn);

	AEnemy* Enemy = Cast<AEnemy>(InPawn);

	if (Enemy)
	{

		BlackboardComp->InitializeBlackboard(*(BehaviourTree->BlackboardAsset));

		BehaviorTreeComp->StartTree(*BehaviourTree);

		BlackboardComp->SetValueAsBool(FName{ "GetAggro" }, true);

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