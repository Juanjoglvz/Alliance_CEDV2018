// Fill out your copyright notice in the Description page of Project Settings.

#include "SerjMaoController.h"
#include "AllianceCharacter.h"
#include "Enemy.h"


ASerjMaoController::ASerjMaoController()
{
	auto TreeAsset = ConstructorHelpers::FObjectFinder<UBehaviorTree>(TEXT("BehaviorTree'/Game/ThirdPersonCPP/Blueprints/Artificial_Intelligence/SerjMao/SerjMao_BT.SerjMao_BT'"));
	BehaviourTree = TreeAsset.Object;
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ASerjMaoController::SensePawn);
}

void ASerjMaoController::Possess(APawn* InPawn)
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

void ASerjMaoController::SensePawn(const TArray<AActor*> &UpdatedActors)
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
