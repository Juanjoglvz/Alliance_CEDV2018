/* Copyright (C) 2018 Iván García, Juan José Corroto and Javier Córdoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

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

	AAllianceCharacter* SerjMao = Cast<AAllianceCharacter>(InPawn);

	if (SerjMao)
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