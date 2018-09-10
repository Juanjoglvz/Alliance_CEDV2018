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
#include "EnemyFactory.generated.h"

UENUM(BlueprintType)	
enum class EEnemyType: uint8
{
	ET_Melee 	UMETA(DisplayName = "Melee"),
	ET_Ranged 	UMETA(DisplayName = "Ranged"),
	ET_Tank		UMETA(DisplayName = "Tank"),
	ET_Euronymous UMETA(DisplayName = "Euronymous"),
	ET_Shiva	UMETA(DisplayName = "Shiva"),
	ET_Henkka	UMETA(DisplayName = "Henkka")
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
	TSubclassOf<class AEnemy> EnemyShivaReference;
	TSubclassOf<class AEnemy> EnemyEuronymousReference;
	TSubclassOf<class AEnemy> EnemyHenkkaReference;
};
