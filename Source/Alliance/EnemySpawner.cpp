// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner.h"
#include "EngineUtils.h"
#include "EnemyFactory.h"
#include "Enemy.h"

// Sets default values
AEnemySpawner::AEnemySpawner() : Super(), NumberOfMeleeEnemiesToSpawn{ 0 }, NumberOfRangedEnemiesToSpawn{ 0 }, NumberOfTankEnemiesToSpawn{ 0 }, NumberOfWaves{ 0 }, SpawnedWaves{ 0 }, b_SpawnNewWave { false },
LocationWhereSpawn(FVector(0.f, 0.f, 0.f)), NumberOfAliveEnemies { 0 }
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<AEnemyFactory> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		EnemyFactory = *ActorItr;
	}
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Spawn a new wave
	if (b_SpawnNewWave && SpawnedWaves < NumberOfWaves && EnemyFactory)
	{
		SpawnedWaves++;
		b_SpawnNewWave = false;

		// Spawn Melee enemies
		for (int i = 0; i < NumberOfMeleeEnemiesToSpawn; i++)
		{
			AEnemy* Enemy = EnemyFactory->SpawnAnEnemy(EEnemyType::ET_Melee, LocationWhereSpawn + FVector(100.f * i, 100.f * i, 0.f),
														FRotator(0.f, 0.f, 0.f), GetUniqueID());
			if (Enemy)
			{
				Enemy->OnEnemyDead.AddDynamic(this, &AEnemySpawner::EnemyDestroyed);
				NumberOfAliveEnemies++;
			}
		}

		// Spawn Ranged enemies
		for (int i = 0; i < NumberOfRangedEnemiesToSpawn; i++)
		{
			AEnemy* Enemy = EnemyFactory->SpawnAnEnemy(EEnemyType::ET_Ranged, LocationWhereSpawn + FVector(500.f + 100.f * i, 500.f + 100.f * i, 0.f), 
														FRotator(0.f, 0.f, 0.f), GetUniqueID());
			if (Enemy) 
			{
				Enemy->OnEnemyDead.AddDynamic(this, &AEnemySpawner::EnemyDestroyed);
				NumberOfAliveEnemies++;
			}
		}

		// Spawn Distance enemies
		for (int i = 0; i < NumberOfTankEnemiesToSpawn; i++)
		{
			AEnemy* Enemy = EnemyFactory->SpawnAnEnemy(EEnemyType::ET_Tank, LocationWhereSpawn + FVector(1000.f + 100.f * i, 1000.f + 100.f * i, 0.f), 
														FRotator(0.f, 0.f, 0.f), GetUniqueID());
			if (Enemy)
			{
				Enemy->OnEnemyDead.AddDynamic(this, &AEnemySpawner::EnemyDestroyed);
				NumberOfAliveEnemies++;
			}
		}
	}

	// When the spawner has finished its work, destroy it
	if (SpawnedWaves >= NumberOfWaves)
	{
		this->Destroy();
	}

}

void AEnemySpawner::EnemyDestroyed(uint32 UniqueID)
{
	if (UniqueID == GetUniqueID())
	{
		NumberOfAliveEnemies--;
		if (NumberOfAliveEnemies == 0)
		{
			b_SpawnNewWave = true;
		}
	}
}

