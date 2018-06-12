// Fill out your copyright notice in the Description page of Project Settings.

#include "Board.h"
#include "Engine/World.h"
#include "Engine.h"
#include "MyTimeline.h"
#include "AllianceCharacter.h"
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

	// Set the selected color for the piece
	ChangeColorToPiece(Pieces[CurrentFocus], FLinearColor(1.f, 0.f, 0.6171f, 1.f));
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

void ABoard::RemovePuzzleBindings()
{
	// Get the player controller to setup the actions
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	while(PlayerController->InputComponent->GetNumActionBindings() > 0)
	{
	/*  
		auto x = PlayerController->InputComponent->GetActionBinding(0);
		FString a = x.ActionName.GetPlainNameString();
		std::string test2 = std::string(TCHAR_TO_UTF8(*a));
		UE_LOG(LogTemp, Warning, TEXT("%s"),*FString(test2.c_str()));
	*/	
		PlayerController->InputComponent->RemoveActionBinding(0);
	}

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

	// If player's piece moves to right, check if the movement is victory
	if (column == 1 && piece->b_IsPlayer && IsVictory())
	{
		// Remove puzzle bindings, so the player is not going to be able to move the pieces
		RemovePuzzleBindings();
		// Set player's movement once the puzzle has been finished
		AAllianceCharacter* Character = Cast<AAllianceCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		Character->SetCharacterMovement(Character->GetCharacterInputComponent());
		// Execute callback, to open the door and setup the camera stuff
		OnMiniGameFinished.Broadcast();
		return;
	}

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
	if (Pieces[CurrentFocus]->rowPosition == WinnerBox.X &&
		Pieces[CurrentFocus]->columnPosition + Pieces[CurrentFocus]->width == WinnerBox.Y)
		return true;
	return false;
}

void ABoard::ChangeColorToPiece(APiece* piece, FLinearColor color)
{
	auto DynamicMaterialInstance = UMaterialInstanceDynamic::Create(piece->PieceMaterial, piece);
	DynamicMaterialInstance->SetVectorParameterValue("Color", color);
	piece->SetMaterial(DynamicMaterialInstance);
}

void ABoard::ChangeFocusToPrevious()
{
	// Set current piece its material
	ChangeColorToPiece(Pieces[CurrentFocus], Pieces[CurrentFocus]->Color);
	
	// Change focus to a new piece
	CurrentFocus = (CurrentFocus + 1) % Pieces.Num();

	// Set the selected material to new piece
	ChangeColorToPiece(Pieces[CurrentFocus], FLinearColor(1.f, 0.f, 0.6171f, 1.f));
}
	
void ABoard::ChangeFocusToNext()
{
	// Set current piece its material
	ChangeColorToPiece(Pieces[CurrentFocus], Pieces[CurrentFocus]->Color);

	// Change focus to a new piece
	if (CurrentFocus - 1 < 0)
	{
		CurrentFocus = Pieces.Num() - 1;
	}
	else
	{
		CurrentFocus -= 1;
	}

	// Set the selected material to new piece
	ChangeColorToPiece(Pieces[CurrentFocus], FLinearColor(1.f, 0.f, 0.6171f, 1.f));
}
