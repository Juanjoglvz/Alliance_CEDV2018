// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeController.h"
#include "AllianceCharacter.h"
#include "Enemy.h"


AMeleeController::AMeleeController()
{
	auto TreeAsset = ConstructorHelpers::FObjectFinder<UBehaviorTree>(TEXT("BehaviorTree'/Game/ThirdPersonCPP/Blueprints/Artificial_Intelligence/MeleeEnemy/MeleeEnemy_BT.MeleeEnemy_BT'"));
	BehaviourTree = TreeAsset.Object;
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AMeleeController::SensePawn);
}

void AMeleeController::Possess(APawn* InPawn)
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

void AMeleeController::SensePawn(const TArray<AActor*> &UpdatedActors)
{
	for (auto& Actor : UpdatedActors)
	{
		AAllianceCharacter* Player = Cast<AAllianceCharacter>(Actor);

		if (Player != nullptr)
		{
			if (BlackboardComp->GetValueAsBool(FName{ "GetAggro" }))
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "I see you!");
				FName Key = FName{ "PlayerAggro" };
				BlackboardComp->SetValueAsObject(Key, Player);
				BlackboardComp->SetValueAsBool(FName{ "GetAggro" }, false);
			}
			break;
		}
	}
}
