/* Copyright (C) 2018 Ivan Garcia, Juan Jose Corroto and Javier Cordoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "EnemySpawner.generated.h"

UCLASS()
class ALLIANCE_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawner)
		int NumberOfMeleeEnemiesToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawner)
		int NumberOfRangedEnemiesToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawner)
		int NumberOfTankEnemiesToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawner)
		bool b_SpawnFirstBoss;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawner)
		bool b_SpawnSecondBoss;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawner)
		bool b_SpawnThirdBoss;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawner)
		int NumberOfWaves;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawner)
		FVector LocationWhereSpawn;
	UPROPERTY(BlueprintReadWrite, Category = EnemySpawner)
		bool b_SpawnNewWave;

private:
	int SpawnedWaves;
	int NumberOfAliveEnemies;
	bool b_BossHasBeenSpawned;
	class AEnemyFactory* EnemyFactory;

	void SpawnEnemies();
	void SpawnBosses();

	UFUNCTION()
		void EnemyDestroyed(uint32 UniqueID);
	UFUNCTION()
		void BossDestroyed(uint32 UniqueID);
};
