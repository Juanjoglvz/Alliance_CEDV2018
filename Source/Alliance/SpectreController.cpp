// Fill out your copyright notice in the Description page of Project Settings.

#include "SpectreController.h"
#include "AllianceCharacter.h"
<<<<<<< HEAD
=======
#include "Enemy.h"

>>>>>>> AI

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

<<<<<<< HEAD
	AAllianceCharacter* SerjMao = Cast<AAllianceCharacter>(InPawn);

	if (SerjMao)
=======
	AAllianceCharacter* Pawn = Cast<AAllianceCharacter>(InPawn);

	if (Pawn)
>>>>>>> AI
	{

		BlackboardComp->InitializeBlackboard(*(BehaviourTree->BlackboardAsset));

		BehaviorTreeComp->StartTree(*BehaviourTree);

<<<<<<< HEAD
		BlackboardComp->SetValueAsBool(FName{ "GetAggro" }, true);

=======
		BlackboardComp->SetValueAsBool(FName{ "InBattle" }, false);

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		APawn* PlayerPawn = PlayerController->GetPawn();

		AAllianceCharacter* Player = Cast<AAllianceCharacter>(PlayerPawn);
		BlackboardComp->SetValueAsObject(FName{ "Player" }, Player);
>>>>>>> AI
	}
}

void ASpectreController::SensePawn(const TArray<AActor*> &UpdatedActors)
{
	for (auto& Actor : UpdatedActors)
	{
<<<<<<< HEAD
		AAllianceCharacter* Player = Cast<AAllianceCharacter>(Actor);

		if (Player != nullptr)
		{
			if (BlackboardComp->GetValueAsBool(FName{ "GetAggro" }))
			{
				FName Key = FName{ "PlayerAggro" };
				BlackboardComp->SetValueAsObject(Key, Player);
				BlackboardComp->SetValueAsBool(FName{ "GetAggro" }, false);
=======
		AEnemy* Enemy = Cast<AEnemy>(Actor);

		if (Enemy != nullptr)
		{
			if (!BlackboardComp->GetValueAsBool(FName{ "InBattle" }))
			{
				BlackboardComp->SetValueAsBool(FName{ "InBattle" }, true);
>>>>>>> AI
			}
			break;
		}
	}
}

<<<<<<< HEAD


=======
>>>>>>> AI
