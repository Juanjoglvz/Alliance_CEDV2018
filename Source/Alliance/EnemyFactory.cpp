// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyFactory.h"
#include "Engine.h"
#include "Enemy.h"

// Sets default values
AEnemyFactory::AEnemyFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	// Get references to enemies blueprints
	static ConstructorHelpers::FObjectFinder<UBlueprint> MeleeBP(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/MeleeEnemy.MeleeEnemy'"));
	if (MeleeBP.Object) {
		EnemyMeleeReference = (UClass*)MeleeBP.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> RangedBP(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/RangedEnemy.RangedEnemy'"));
	if (RangedBP.Object) {
		EnemyRangedReference = (UClass*)RangedBP.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> TankBP(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/TankEnemy.TankEnemy'"));
	if (TankBP.Object) {
		EnemyTankReference = (UClass*)TankBP.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void AEnemyFactory::BeginPlay()
{
	Super::BeginPlay();
	
	Instigator = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	SetOwner(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AEnemyFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AEnemy* AEnemyFactory::SpawnAnEnemy(EEnemyType EnemyType, FVector Location, FRotator Rotation, uint32 SpawnerUniqueID)
{
	AEnemy* EnemyToReturn;
	if (GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (EnemyType == EEnemyType::ET_Melee)
		{
			EnemyToReturn = GetWorld()->SpawnActor<AEnemy>(EnemyMeleeReference, Location, Rotation, SpawnParams);
		}
		else if (EnemyType == EEnemyType::ET_Ranged)
		{
			EnemyToReturn = GetWorld()->SpawnActor<AEnemy>(EnemyRangedReference, Location, Rotation, SpawnParams);
		}
		else if (EnemyType == EEnemyType::ET_Tank)
		{
			EnemyToReturn = GetWorld()->SpawnActor<AEnemy>(EnemyTankReference, Location, Rotation, SpawnParams);
		}
		else
		{
			EnemyToReturn = nullptr;
		}
	}
	else
	{
		EnemyToReturn = nullptr;
	}

	if (EnemyToReturn)
	{
		EnemyToReturn->SpawnerUniqueID = SpawnerUniqueID;
	}

	return EnemyToReturn;
}


