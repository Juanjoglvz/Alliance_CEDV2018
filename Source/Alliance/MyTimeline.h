// Fill out your copyright notice in the Description page of Project Settings.

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

	UPROPERTY()
		UTimelineComponent* MyTimeline;

	UPROPERTY()
		UCurveFloat* FloatCurve;

	UFUNCTION()
		void TimelineCallback(float val);

	UFUNCTION()
		void TimelineFinishedCallback();

	UPROPERTY()
		TEnumAsByte<ETimelineDirection::Type> TimelineDirection;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPiece(class APiece* Piece, int col, int row);

	void PlayTimeline();
	
private:
	TWeakObjectPtr<APiece> Piece;
	int Col;
	int Row;
};
