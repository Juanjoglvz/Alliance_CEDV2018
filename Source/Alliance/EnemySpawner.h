// Fill out your copyright notice in the Description page of Project Settings.

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
		int NumberOfWaves;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawner)
		FVector LocationWhereSpawn;
	UPROPERTY(BlueprintReadWrite, Category = EnemySpawner)
		bool b_SpawnNewWave;

private:
	int SpawnedWaves;
	int NumberOfAliveEnemies;
	class AEnemyFactory* EnemyFactory;

	UFUNCTION()
	void EnemyDestroyed(uint32 UniqueID);
};
