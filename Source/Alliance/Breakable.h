/* Copyright (C) 2018 Iván García, Juan José Corroto and Javier Córdoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Breakable.generated.h"

/**
 * 
 */
UCLASS()
class ALLIANCE_API ABreakable : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
		class UStaticMesh* BreakableMesh;
	UPROPERTY(EditAnywhere)
		class UStaticMesh* BreakableMesh_broken;
	UPROPERTY(EditAnywhere)
		int HealthSpawnProbability;
	UPROPERTY(BlueprintReadWrite)
		UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(Replicated)
		bool b_IsBroken;
	UPROPERTY(Replicated)
		bool b_Overlaping;

	// Delegate executed when the breakable is hit by Morten
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMortenHit, class AAllianceCharacter*, Morten);
	UPROPERTY(BlueprintReadWrite)
		FOnMortenHit OnMortenHitDelegate;

	UFUNCTION(BlueprintCallable)
		void ExecuteOnMortenHitDelegate(class AAllianceCharacter* Morten);

	// Functions to replicate over the network the actor is broken
	UFUNCTION(Reliable, Server, WithValidation)
		void OnServerIsBroken();
	void OnServerIsBroken_Implementation();
	FORCEINLINE bool OnServerIsBroken_Validate() { return true; }

	UFUNCTION(Reliable, NetMulticast)
		void ExecuteWhenBroken();
	void ExecuteWhenBroken_Implementation();
	void BreakableIsBroken();

	virtual void Tick(float DeltaSeconds) override;

	ABreakable();
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
		void OnAlyssaBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnAlyssaEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	bool b_Success;

	UFUNCTION()
		void OnMortenHit(class AAllianceCharacter* Morten);

	void RandomDrop(AAllianceCharacter* Character);
	
	// Pickup assets
	UPROPERTY()
		class UStaticMesh* Health_Asset;
	UPROPERTY()
		class UStaticMesh* Stamina_Asset;

	// Overlaping character reference
	UPROPERTY()
		class AAllianceCharacter* OverlapingCharacter;

	// Breakable sound when destroyed
	UPROPERTY()
		class UAudioComponent* breakableAudioComponent;

};
