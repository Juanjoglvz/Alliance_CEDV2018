// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
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
	
	// Board properties
	UPROPERTY(EditAnywhere)
		int NumberOfRows;
	UPROPERTY(EditAnywhere)
		int NumberOfColumns;
	// Reference to all the pieces the board has
	UPROPERTY(EditAnywhere)
		TArray<APiece*> Pieces;
	// Location the player has to reach to win the game
	UPROPERTY(EditAnywhere)
		FVector2D WinnerBox;

	// Delegate executed when the minigame has finished. 
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameFinished);
	UPROPERTY(BlueprintAssignable, Category = "MiniGame")
		FGameFinished OnMiniGameFinished;

private:
	// Index to indicate the piece which will move if the player makes a movement action
	int CurrentFocus;

	// Board representation. If a cell is false, it hasn't a piece on it. Otherwise, the cell will be true.
	TBitArray<> Representation{ false, 0 };

	// Setup player's input to move and select pieces in the board
	UFUNCTION(BlueprintCallable)
		void SetupInputComponent();

	// This function avoid the player to move pieces when the minigame is not active
	void RemovePuzzleBindings();
	
	// Methods to move a piece through the board
	void MoveTo(Movement m_movement);
	void MovePieceToRowAndColumn(int row, int column);
	
	template<Movement m_movement> 
		void MoveTo() { MoveTo(m_movement); }

	// Function used to know if the movement is a player victory
	bool IsVictory();

	// These methods change the piece the player is going to move
	void ChangeFocusToNext();
	void ChangeFocusToPrevious();

	// Methods to check if a piece can move to a given cell
	int GetBoardCoordinates(int row, int column);
	bool PieceCanMoveTo(int left, int up);

};
