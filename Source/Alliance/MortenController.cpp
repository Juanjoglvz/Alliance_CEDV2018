// Fill out your copyright notice in the Description page of Project Settings.

#include "MortenController.h"
#include "AllianceCharacter.h"
#include "Enemy.h"


AMortenController::AMortenController()
{
	auto TreeAsset = ConstructorHelpers::FObjectFinder<UBehaviorTree>(TEXT("BehaviorTree'/Game/ThirdPersonCPP/Blueprints/Artificial_Intelligence/Morten/Morten_BT.Morten_BT'"));
	BehaviourTree = TreeAsset.Object;
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AMortenController::SensePawn);
}

void AMortenController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, SightConfig->GetSenseImplementation(), InPawn);

	AAllianceCharacter* Pawn = Cast<AAllianceCharacter>(InPawn);

	if (Pawn)
	{

		BlackboardComp->InitializeBlackboard(*(BehaviourTree->BlackboardAsset));

		BehaviorTreeComp->StartTree(*BehaviourTree);

		BlackboardComp->SetValueAsBool(FName{ "InBattle" }, false);

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		APawn* PlayerPawn = PlayerController->GetPawn();

		AAllianceCharacter* Player = Cast<AAllianceCharacter>(PlayerPawn);
		BlackboardComp->SetValueAsObject(FName{ "Player" }, Player);
		BlackboardComp->SetValueAsObject(FName{ "EnemyAggro" }, nullptr);
	}
}

void AMortenController::SensePawn(const TArray<AActor*> &UpdatedActors)
{
	for (auto& Actor : UpdatedActors)
	{
		AEnemy* Enemy = Cast<AEnemy>(Actor);

		if (Enemy != nullptr)
		{
			if (!BlackboardComp->GetValueAsBool(FName{ "InBattle" }))
			{
				BlackboardComp->SetValueAsBool(FName{ "InBattle" }, true);

				if (BlackboardComp->GetValueAsObject(FName{ "EnemyAggro" }) == nullptr)
				{
					BlackboardComp->SetValueAsObject(FName{ "EnemyAggro" }, Enemy);
				}

			}

			if (BlackboardComp->GetValueAsObject(FName{ "EnemyAggro" }) == nullptr)
			{
				BlackboardComp->SetValueAsObject(FName{ "EnemyAggro" }, Enemy);
			}

			break;
		}
	}
}
