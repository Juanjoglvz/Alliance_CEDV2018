// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeController.h"
#include "Enemy.h"


AMeleeController::AMeleeController()
{
	auto TreeAsset = ConstructorHelpers::FObjectFinder<UBehaviorTree>(TEXT("BehaviorTree'/Game/ThirdPersonCPP/Blueprints/Artificial_Intelligence/MeleeEnemy_BT.MeleeEnemy_BT'"));
	BehaviourTree = TreeAsset.Object;
}

void AMeleeController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AEnemy* Enemy = Cast<AEnemy>(InPawn);

	if (Enemy)
	{
		BehaviorTreeComp->StartTree(*BehaviourTree);
	}
}
