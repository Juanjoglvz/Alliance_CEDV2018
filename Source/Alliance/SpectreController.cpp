// Fill out your copyright notice in the Description page of Project Settings.

#include "SpectreController.h"
#include "AllianceCharacter.h"

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

	AAllianceCharacter* SerjMao = Cast<AAllianceCharacter>(InPawn);

	if (SerjMao)
	{

		BlackboardComp->InitializeBlackboard(*(BehaviourTree->BlackboardAsset));

		BehaviorTreeComp->StartTree(*BehaviourTree);

		BlackboardComp->SetValueAsBool(FName{ "GetAggro" }, true);

	}
}

void ASpectreController::SensePawn(const TArray<AActor*> &UpdatedActors)
{
	for (auto& Actor : UpdatedActors)
	{
		AAllianceCharacter* Player = Cast<AAllianceCharacter>(Actor);

		if (Player != nullptr)
		{
			if (BlackboardComp->GetValueAsBool(FName{ "GetAggro" }))
			{
				FName Key = FName{ "PlayerAggro" };
				BlackboardComp->SetValueAsObject(Key, Player);
				BlackboardComp->SetValueAsBool(FName{ "GetAggro" }, false);
			}
			break;
		}
	}
}


