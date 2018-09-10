/* Copyright (C) 2018 Ivan Garcia, Juan Jose Corroto and Javier Cordoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#include "SpectreController.h"
#include "AllianceCharacter.h"
#include "Enemy.h"


ASpectreController::ASpectreController()
{
	auto TreeAsset = ConstructorHelpers::FObjectFinder<UBehaviorTree>(TEXT("BehaviorTree'/Game/ThirdPersonCPP/Blueprints/Artificial_Intelligence/Spectre/Spectre_BT.Spectre_BT'"));
	BehaviourTree = TreeAsset.Object;
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ASpectreController::SensePawn);
}

void ASpectreController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, SightConfig->GetSenseImplementation(), InPawn);

	AAllianceCharacter* Spectre = Cast<AAllianceCharacter>(InPawn);

	if (Spectre)
	{

		BlackboardComp->InitializeBlackboard(*(BehaviourTree->BlackboardAsset));

		BehaviorTreeComp->StartTree(*BehaviourTree);
		BlackboardComp->SetValueAsBool(FName{ "InBattle" }, false);

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		APawn* PlayerPawn = PlayerController->GetPawn();

		AAllianceCharacter* Player = Cast<AAllianceCharacter>(PlayerPawn);
		BlackboardComp->SetValueAsObject(FName{ "Player" }, Player);
	}
}

void ASpectreController::SensePawn(const TArray<AActor*> &UpdatedActors)
{
	for (auto& Actor : UpdatedActors)
	{
		AEnemy* Enemy = Cast<AEnemy>(Actor);

		if (Enemy != nullptr)
		{
			if (!BlackboardComp->GetValueAsBool(FName{ "InBattle" }))
			{
				BlackboardComp->SetValueAsBool(FName{ "InBattle" }, true);
			}
			break;
		}
	}
}
