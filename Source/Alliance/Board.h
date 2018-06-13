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
	UPROPERTY(EditAnywhere)
		FVector2D WinnerBox;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameFinished);
	UPROPERTY(BlueprintAssignable, Category = "MiniGame")
		FGameFinished OnMiniGameFinished;

private:

	int CurrentFocus;
	std::vector<bool> Representation;

	// Setup player's input to move and select pieces in the board
	UFUNCTION(BlueprintCallable)
		void SetupInputComponent();

	void RemovePuzzleBindings();
	
	// Methods to move a piece through the board
	void MoveTo(Movement m_movement);
	void MovePieceToRowAndColumn(int row, int column);
	
	template<Movement m_movement> 
		void MoveTo() { MoveTo(m_movement); }

	bool IsVictory();

	// These methods change the piece the player is going to move
	void ChangeFocusToNext();
	void ChangeFocusToPrevious();

	// Methods to check if a piece can move to a given cell
	int GetBoardCoordinates(int row, int column);
	bool PieceCanMoveTo(int left, int up);

};
