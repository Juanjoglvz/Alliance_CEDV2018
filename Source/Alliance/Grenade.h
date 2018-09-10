/* Copyright (C) 2018 Ivan Garcia, Juan Jose Corroto and Javier Cordoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

UCLASS()
class ALLIANCE_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY()
		UProjectileMovementComponent* PMovementComponent;
	UPROPERTY()
		UParticleSystemComponent* PSystemComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function called when timer ends and grenade explodes
	UFUNCTION()
		void Explode();

private:
	float Dmg;
	float Timer;
	
};
