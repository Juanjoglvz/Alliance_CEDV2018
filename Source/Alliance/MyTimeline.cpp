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
		MyTimeline = NewObject<UTimelineComponent>(this, FName("TimelineAnimation"));
		MyTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript; // Indicate it comes from a blueprint so it gets cleared when we rerun construction scripts
		this->BlueprintCreatedComponents.Add(MyTimeline); // Add to array so it gets saved
		MyTimeline->SetNetAddressable();	// This component has a stable name that can be referenced for replication

		MyTimeline->SetPropertySetObject(this); // Set which object the timeline should drive properties on
		MyTimeline->SetDirectionPropertyName(FName("TimelineDirection"));

		MyTimeline->SetLooping(false);
		MyTimeline->SetTimelineLength(0.2f);
		MyTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

		MyTimeline->SetPlaybackPosition(0.0f, false);

		//Add the float curve to the timeline and connect it to your timelines's interpolation function
		onTimelineCallback.BindUFunction(this, FName{ TEXT("TimelineCallback") });
		onTimelineFinishedCallback.BindUFunction(this, FName{ TEXT("TimelineFinishedCallback") });
		MyTimeline->AddInterpFloat(FloatCurve, onTimelineCallback);
		MyTimeline->SetTimelineFinishedFunc(onTimelineFinishedCallback);

		MyTimeline->RegisterComponent();
	}
	
}

// Called every frame
void AMyTimeline::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (MyTimeline != NULL)
	{
		MyTimeline->TickComponent(deltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
	}
}

void AMyTimeline::TimelineCallback(float interpolatedVal)
{
	this->Piece->SetActorLocation(FVector(Piece->GetActorLocation().X - (interpolatedVal * Col),
										Piece->GetActorLocation().Y - (interpolatedVal * Row),
										Piece->GetActorLocation().Z));
}

void AMyTimeline::SetPiece(APiece* Piece, int col, int row)
{
	this->Piece = Piece;
	this->Col = col;
	this->Row = row;
}

void AMyTimeline::TimelineFinishedCallback()
{
	this->Destroy();
}

void AMyTimeline::PlayTimeline()
{
	if (MyTimeline != NULL)
	{
		MyTimeline->PlayFromStart();
	}
}

