// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyController.h"

AEnemyController::AEnemyController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}


void AEnemyController::Tick(float DeltaTime)
{

}

void AEnemyController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	SightConfig->SightRadius = 3000.f;
	SightConfig->LoseSightRadius = 3200.f;
	SightConfig->PeripheralVisionAngleDegrees = 50.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	PerceptionComponent->ConfigureSense(*SightConfig);

}
