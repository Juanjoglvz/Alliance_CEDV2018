// Fill out your copyright notice in the Description page of Project Settings.

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
	UPROPERTY(BlueprintReadWrite)
		UStaticMeshComponent* StaticMeshComponent;

	// Delegate executed when the breakable is hit by Morten
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMortenHit, class AAllianceCharacter*, Morten);
	UPROPERTY(BlueprintReadWrite)
		FOnMortenHit OnMortenHitDelegate;

	UFUNCTION(BlueprintCallable)
		void ExecuteOnMortenHitDelegate(class AAllianceCharacter* Morten);

	virtual void Tick(float DeltaSeconds) override;

	ABreakable();
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnAlyssaBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnAlyssaEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
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

	bool b_IsBroken;
	bool b_Overlaping;

};
