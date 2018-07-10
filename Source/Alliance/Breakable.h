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

	ABreakable();
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnAlyssaHit(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UFUNCTION()
		void OnMortenHit(class AAllianceCharacter* Morten);

	void RandomDrop(AAllianceCharacter* Character);
	class UStaticMesh* Health_Asset;
	class UStaticMesh* Stamina_Asset;

	bool b_IsBroken;
};
