// Fill out your copyright notice in the Description page of Project Settings.

#include "Board.h"
#include "Engine/World.h"
#include "MyTimeline.h"
#include "Math/IntPoint.h"


// Sets default values
ABoard::ABoard() : CurrentFocus(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();
	
	// Setup the actions that are going to be executed when player press a certain key         
	SetupInputComponent();
}

// Called every frame
void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoard::SetupInputComponent()
{
	// Get the player controller to setup the actions
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	EnableInput(PlayerController);

	// Setup the input actions
	PlayerController->InputComponent->BindAction("PreviousPiece", EInputEvent::IE_Pressed, this, &ABoard::ChangeFocusToPrevious);
	PlayerController->InputComponent->BindAction("NextPiece", EInputEvent::IE_Pressed, this, &ABoard::ChangeFocusToNext);
	PlayerController->InputComponent->BindAction("MovePieceUp", EInputEvent::IE_Pressed, this, &ABoard::MoveTo<Movement::MoveUp>).bConsumeInput = false;
	PlayerController->InputComponent->BindAction("MovePieceDown", EInputEvent::IE_Pressed, this, &ABoard::MoveTo<Movement::MoveDown>).bConsumeInput = false;
	PlayerController->InputComponent->BindAction("MovePieceRight", EInputEvent::IE_Pressed, this, &ABoard::MoveTo<Movement::MoveRight>).bConsumeInput = false;
	PlayerController->InputComponent->BindAction("MovePieceLeft", EInputEvent::IE_Pressed, this, &ABoard::MoveTo<Movement::MoveLeft>).bConsumeInput = false;
}

void ABoard::MoveTo(Movement m_movement)
{
	switch (m_movement)
	{
		case Movement::MoveUp:
			MovePieceToRowAndColumn(-1, 0);
			break;

		case Movement::MoveDown:
			MovePieceToRowAndColumn(1, 0);
			break;

		case Movement::MoveLeft:
			MovePieceToRowAndColumn(0, -1);
			break;

		case Movement::MoveRight:
			MovePieceToRowAndColumn(0, 1);
			break;
	}
}

void ABoard::MovePieceToRowAndColumn(int row, int column)
{
	if (CurrentFocus >= Pieces.Num())
		return;

	APiece* piece = Pieces[CurrentFocus];

	if (PieceCanMoveTo(column, row, piece))
	{
		FIntPoint movement(column, row);
		FIntRect OriginalRectangle = PieceToRectangle(piece);

		// Rectangle resulting of moving the piece in the desired direction
		FIntRect NewRectangle = OriginalRectangle;

		// TODO: Why minus?
		NewRectangle.Min -= movement;
		NewRectangle.Max -= movement;

		// Update boolean matrix
		for (int i = OriginalRectangle.Min.X; i < OriginalRectangle.Max.X; i++)
		{
			for (int j = OriginalRectangle.Min.Y; i < OriginalRectangle.Max.Y; j++)
			{
				Representation[GetBoardCoordinates(i, j)] = false;
			}
		}

		for (int i = NewRectangle.Min.X; i < NewRectangle.Max.X; i++)
		{
			for (int j = NewRectangle.Min.Y; i < NewRectangle.Max.Y; j++)
			{
				Representation[GetBoardCoordinates(i, j)] = true;
			}
		}

		// Move the Piece in the world
		AMyTimeline* Timeline = GetWorld()->SpawnActor<AMyTimeline>();
		Timeline->SetPiece(piece, column, row);
		Timeline->PlayTimeline();

		// Update the Piece's position
		piece->columnPosition += column;
		piece->rowPosition += row;
	}
}

bool ABoard::PieceCanMoveTo(int left, int up, APiece* piece)
{
	FIntPoint movement(up, left);
	FIntRect OriginalRectangle = PieceToRectangle(piece);

	FIntRect NewRectangle = OriginalRectangle;

	NewRectangle.Min += movement;
	NewRectangle.Max += movement;

	if (NewRectangle.Min.X < 0 || NewRectangle.Min.Y < 0 || NewRectangle.Max.X >= NumberOfRows || NewRectangle.Max.Y >= NumberOfColumns)
		return false;

	for (int i = NewRectangle.Min.X; i < NewRectangle.Max.X; i++)
	{
		for (int j = NewRectangle.Min.Y; i < NewRectangle.Max.Y; j++)
		{
			if (Representation[GetBoardCoordinates(i, j)])
				return false;
		}
	}

	return true;
}

FIntRect ABoard::PieceToRectangle(APiece* piece)
{
	return FIntRect(piece->rowPosition, piece->columnPosition,
		piece->rowPosition + piece->height - 1, piece->columnPosition + piece->width - 1);
}

int ABoard::GetBoardCoordinates(int row, int column)
{
	return row * NumberOfColumns + column - 1;
}

bool ABoard::IsVictory()
{
	return true;
}

void ABoard::ChangeFocusToPrevious()
{
	CurrentFocus = (CurrentFocus + 1) % Pieces.Num();
}

void ABoard::ChangeFocusToNext()
{
	CurrentFocus = (CurrentFocus - 1) % Pieces.Num();
}
