// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UENUM(BlueprintType)
enum class EPickup : uint8
{
	Health_Pickup 	UMETA(DisplayName = "Health"),
	Stamina_Pickup 	UMETA(DisplayName = "Stamina")
};

UCLASS()
class ALLIANCE_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	
	UPROPERTY(Replicated)
		EPickup pickupType;

	// Sets default values for this actor's properties
	APickup();

	void SetStaticMeshAsset(UStaticMesh* StaticMeshAsset);
	FORCEINLINE void SetPickupType(EPickup pickup_type) { pickupType = pickup_type; }

	// Functions to replicate over the network the actor is broken
	UFUNCTION(Reliable, Server, WithValidation)
		void OnServerIsBroken();
	void OnServerIsBroken_Implementation();
	FORCEINLINE bool OnServerIsBroken_Validate() { return true; }

	UFUNCTION(Reliable, NetMulticast)
		void ExecuteWhenBroken();
	void ExecuteWhenBroken_Implementation();
	void BreakableIsBroken();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY()
		class UStaticMeshComponent* StaticMeshComponent;
	UFUNCTION()
		void OnOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	// Shooting sound
	UPROPERTY()
		UAudioComponent* pickupAudioComponent;
};
