// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyFactory.generated.h"

UENUM(BlueprintType)	
enum class EEnemyType: uint8
{
	ET_Melee 	UMETA(DisplayName = "Melee"),
	ET_Ranged 	UMETA(DisplayName = "Ranged"),
	ET_Tank		UMETA(DisplayName = "Tank")
};

UCLASS()
class ALLIANCE_API AEnemyFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class AEnemy* SpawnAnEnemy(EEnemyType EnemyType, FVector Location, FRotator Rotation, uint32 SpawnerUniqueID);
	
private:
	TSubclassOf<class AEnemy> EnemyTankReference;
	TSubclassOf<class AEnemy> EnemyRangedReference;
	TSubclassOf<class AEnemy> EnemyMeleeReference;
};
