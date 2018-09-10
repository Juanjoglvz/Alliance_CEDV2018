/* Copyright (C) 2018 Ivan Garcia, Juan Jose Corroto and Javier Cordoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#pragma once

#include "CoreMinimal.h"
#include "Math/Color.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "Piece.generated.h"



UCLASS()
class ALLIANCE_API APiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APiece();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Piece properties
	UPROPERTY(EditAnywhere)
		bool b_IsPlayer;
	UPROPERTY(EditAnywhere)
		int width;
	UPROPERTY(EditAnywhere)
		int height;
	UPROPERTY(EditAnywhere)
		int rowPosition;
	UPROPERTY(EditAnywhere)
		int columnPosition;
	
	UPROPERTY(EditAnywhere)
		class UMaterialInstance* PieceMaterial;
	UPROPERTY(EditAnywhere)
		class UStaticMesh* PieceMesh;

	// This variable stores the color of the piece. It is used to return the original color to the piece when it hasn't the focus
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FLinearColor Color;

	// Timeline properties and functions
	UPROPERTY()
		UCurveFloat* FloatCurve;
	UPROPERTY()
		TEnumAsByte<ETimelineDirection::Type> TimelineDirection;
	// Variable to control if the timeline has finished
	bool TimelineFinished = true;
	
	// Function executed when the timeline is called. The function is binded with onTimelineCallback event
	UFUNCTION()
		void TimelineCallback(float val);
	// Function called when the timeline execution has finished. The function is binded with onTimelineFinishedCallback event
	UFUNCTION()
		void TimelineFinishedCallback();
	
	/**
	 * Function called by the board when a piece has to be moved
	 * @param col A -1 value indicates move to the left. A +1 value indicates move to the right
	 * @param row A -1 value indicates move down. A +1 value indicates move up
	 */
	UFUNCTION(BlueprintCallable)
	void PlayTimeline(int col, int row);


	// RPC to move pieces. Every client will call ExecutingTimeline_Implementation
	// The function ExecutingTimeline doesn't need an implementation
	UFUNCTION(Reliable, NetMulticast)
		void ExecutingTimeline(float interpolatedVal);
	void ExecutingTimeline_Implementation(float interpolatedVal);

	// RPC to change pieces color
	UFUNCTION(Reliable, NetMulticast)
		void ExecuteChangeColor(FLinearColor NewColor);
	void ExecuteChangeColor_Implementation(FLinearColor NewColor);

	// This method changes the color of a given piece in the board
	UFUNCTION(BlueprintCallable)
	void ChangeColor(FLinearColor NewColor);

	// Change pieces material in runtime
	void SetMaterial(class UMaterialInstanceDynamic* NewMaterial);

	FIntRect PieceToRectangle();

private:
	UPROPERTY()
		UStaticMeshComponent* PieceMeshComponent;
	
	// Timeline property
	FTimeline MyTimeline;

	// Piece's location before the it moves. 
	FVector StartingPosition;

	int row, col;
};
