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
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "MyTimeline.generated.h"


UCLASS()
class ALLIANCE_API AMyTimeline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyTimeline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*UPROPERTY()
		UTimelineComponent* MyTimeline;*/

	FVector StartingPosition;

	UPROPERTY()
		UCurveFloat* FloatCurve;

	UFUNCTION()
		void TimelineCallback(float val);

	UFUNCTION()
		void TimelineFinishedCallback();

	// RPC to move pieces. Every client will call ExecutingTimeline_Implementation
	// This function doesn't need an implementation
	UFUNCTION(Reliable, NetMulticast)
		void ExecutingTimeline(float interpolatedVal);
	void ExecutingTimeline_Implementation(float interpolatedVal);

	UPROPERTY()
		TEnumAsByte<ETimelineDirection::Type> TimelineDirection;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPiece(class APiece* Piece, int col, int row);

	void PlayTimeline();

	bool Finished = true;
	
private:
	FTimeline MyTimeline;

	TWeakObjectPtr<APiece> Piece;
	int Col;
	int Row;
};
