// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)		//State of the character (Maybe some character does not use all of them)
enum class EEnemyState : uint8
{
	S_Running	UMETA(DisplayName = "Running"),
	S_Idle		UMETA(DisplayName = "Idle"),
	S_Attacking UMETA(DisplayName = "Attacking")
};


UCLASS()
class ALLIANCE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	uint32 SpawnerUniqueID;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDead, uint32, UniqueID);
	UPROPERTY()
		FOnEnemyDead OnEnemyDead;

	// Public atributes for accessing in blueprints

	UPROPERTY(BlueprintReadWrite)
		EEnemyState CurrentState;
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Combat")
		float Damage;
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Combat")
		float Health;

	// Public functions for accessing in blueprints

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void DoDmg(AActor* DamagedActor);
	
	// Reduces the amount of character's health
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) override;

};
