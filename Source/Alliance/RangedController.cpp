// Fill out your copyright notice in the Description page of Project Settings.

#include "RangedController.h"
#include "Enemy.h"


ARangedController::ARangedController()
{
	auto TreeAsset = ConstructorHelpers::FObjectFinder<UBehaviorTree>(TEXT("BehaviorTree'/Game/ThirdPersonCPP/Blueprints/Artificial_Intelligence/RangedEnemy/RangedEnemy_BT.RangedEnemy_BT'"));
	BehaviourTree = TreeAsset.Object;
}

void ARangedController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AEnemy* Enemy = Cast<AEnemy>(InPawn);

	if (Enemy)
	{

		BlackboardComp->InitializeBlackboard(*(BehaviourTree->BlackboardAsset));

		BehaviorTreeComp->StartTree(*BehaviourTree);

	}
}



