// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyController.h"

AEnemyController::AEnemyController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
}


void AEnemyController::Tick(float DeltaTime)
{

}

void AEnemyController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

}
