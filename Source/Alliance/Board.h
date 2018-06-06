// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>
#include "Piece.h"
#include "Board.generated.h"

enum class Movement
{
	MoveRight,
	MoveLeft,
	MoveUp, 
	MoveDown
};

UCLASS()
class ALLIANCE_API ABoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
		int NumberOfRows;
	UPROPERTY(EditAnywhere)
		int NumberOfColumns;
	UPROPERTY(EditAnywhere)
		TArray<APiece*> Pieces;

private:
	int CurrentFocus;
	std::vector<bool> Representation;

	void SetupInputComponent();
	void MoveTo(Movement m_movement);
	void MovePieceToRowAndColumn(int row, int column);
	
	template<Movement m_movement> 
	void MoveTo() { MoveTo(m_movement); }

	bool IsVictory();
	void ChangeFocusToNext();
	void ChangeFocusToPrevious();

	FIntRect PieceToRectangle(APiece* piece);
	int GetBoardCoordinates(int row, int column);
	bool PieceCanMoveTo(int left, int up, APiece* piece);

};
