// Fill out your copyright notice in the Description page of Project Settings.

#include "TankController.h"
#include "Enemy.h"


ATankController::ATankController()
{
	auto TreeAsset = ConstructorHelpers::FObjectFinder<UBehaviorTree>(TEXT("BehaviorTree'/Game/ThirdPersonCPP/Blueprints/Artificial_Intelligence/TankEnemy/TankEnemy_BT.TankEnemy_BT'"));
	BehaviourTree = TreeAsset.Object;
}

void ATankController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AEnemy* Enemy = Cast<AEnemy>(InPawn);

	if (Enemy)
	{

		BlackboardComp->InitializeBlackboard(*(BehaviourTree->BlackboardAsset));

		BehaviorTreeComp->StartTree(*BehaviourTree);

	}
}



