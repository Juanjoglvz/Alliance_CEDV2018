// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTimeline.h"
#include "UObject/ConstructorHelpers.h"
#include "Piece.h"


// Sets default values
AMyTimeline::AMyTimeline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UCurveFloat>Curve(TEXT("CurveFloat'/Game/ThirdPersonCPP/Blueprints/C_MyCurve.C_MyCurve'"));
	check(Curve.Succeeded());

	FloatCurve = Curve.Object;

}

// Called when the game starts or when spawned
void AMyTimeline::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat onTimelineCallback;
	FOnTimelineEventStatic onTimelineFinishedCallback;

	if (FloatCurve != NULL)
	{
		onTimelineCallback.BindUFunction(this, FName{ TEXT("TimelineCallback") });
		onTimelineFinishedCallback.BindUFunction(this, FName{ TEXT("TimelineFinishedCallback") });

		MyTimeline.AddInterpFloat(FloatCurve, onTimelineCallback);
		MyTimeline.SetLooping(false);

		MyTimeline.SetTimelineLength(0.2f);
		MyTimeline.SetTimelineFinishedFunc(onTimelineFinishedCallback);
	}	
}

// Called every frame
void AMyTimeline::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	MyTimeline.TickTimeline(deltaTime);
}

void AMyTimeline::TimelineCallback(float interpolatedVal)
{
	this->Piece->SetActorLocation(FVector(this->StartingPosition.X - (interpolatedVal * Col),
											this->StartingPosition.Y - (interpolatedVal * Row),
											this->StartingPosition.Z));
}

void AMyTimeline::SetPiece(APiece* Piece, int col, int row)
{
	this->Piece = Piece;
	this->Col = col;
	this->Row = row;
	this->StartingPosition = Piece->GetActorLocation();
}

void AMyTimeline::TimelineFinishedCallback()
{
	this->Finished = true;
}

void AMyTimeline::PlayTimeline()
{
	this->Finished = false;
		MyTimeline.PlayFromStart();
}

