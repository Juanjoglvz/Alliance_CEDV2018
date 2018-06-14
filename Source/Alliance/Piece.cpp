// Fill out your copyright notice in the Description page of Project Settings.

#include "Piece.h"
#include "UnrealNetwork.h"
#include "Engine.h"
#include "Math/IntPoint.h"


// Sets default values
APiece::APiece() : b_IsPlayer(false), width{ 1 }, height{ 1 }
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create static mesh component
	PieceMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceMesh"));
	PieceMeshComponent->SetIsReplicated(true);
	
	// Timeline
	static ConstructorHelpers::FObjectFinder<UCurveFloat>Curve(TEXT("CurveFloat'/Game/ThirdPersonCPP/Blueprints/C_MyCurve.C_MyCurve'"));
	check(Curve.Succeeded());
	FloatCurve = Curve.Object;
	
	// Replicate the pieces movement
	SetReplicateMovement(true);
	SetReplicates(true);
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void APiece::BeginPlay()
{
	Super::BeginPlay();

	PieceMeshComponent->SetStaticMesh(PieceMesh);	
	PieceMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	PieceMeshComponent->SetMaterial(0, PieceMaterial);
	PieceMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	if (PieceMaterial)
	{
		auto DynamicMaterialInstance = UMaterialInstanceDynamic::Create(PieceMaterial, this);
		DynamicMaterialInstance->SetVectorParameterValue("Color", Color);
		SetMaterial(DynamicMaterialInstance);
	}

	FOnTimelineFloat onTimelineCallback;
	FOnTimelineEventStatic onTimelineFinishedCallback;

	// Bind Timeline events to C++ functions
	if (FloatCurve)
	{
		onTimelineCallback.BindUFunction(this, FName{ TEXT("TimelineCallback") });
		onTimelineFinishedCallback.BindUFunction(this, FName{ TEXT("TimelineFinishedCallback") });

		MyTimeline.AddInterpFloat(FloatCurve, onTimelineCallback);
		MyTimeline.SetLooping(false);

		MyTimeline.SetTimelineLength(0.2f);
		MyTimeline.SetTimelineFinishedFunc(onTimelineFinishedCallback);
	}
}

void APiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MyTimeline.TickTimeline(DeltaTime);
}

void APiece::SetMaterial(UMaterialInstanceDynamic* NewMaterial)
{
	PieceMeshComponent->SetMaterial(0, NewMaterial);
}

void APiece::TimelineCallback(float val)
{
	ExecutingTimeline(val);
}

void APiece::TimelineFinishedCallback()
{
	TimelineFinished = true;
}

// Update the piece location when the piece is moving. The location is updated by the server and replicated to all the clients.
void APiece::ExecutingTimeline_Implementation(float interpolatedVal)
{
	SetActorLocation(FVector(this->StartingPosition.X - (interpolatedVal * col),
		this->StartingPosition.Y - (interpolatedVal * row),
		this->StartingPosition.Z));
}

// The color is changed in the server and replicated to all the clients
void APiece::ExecuteChangeColor_Implementation(FLinearColor NewColor)
{
	auto DynamicMaterialInstance = UMaterialInstanceDynamic::Create(this->PieceMaterial, this);
	DynamicMaterialInstance->SetVectorParameterValue("Color", NewColor);
	this->SetMaterial(DynamicMaterialInstance);
}


void APiece::PlayTimeline(int col, int row)
{
	TimelineFinished = false;
	this->StartingPosition = this->GetActorLocation();
	this->col = col;
	this->row = row;

	MyTimeline.PlayFromStart();
}

void APiece::ChangeColor(FLinearColor color)
{
	if (GIsServer)
	{
		ExecuteChangeColor(color);
	}
}

FIntRect APiece::PieceToRectangle()
{
	return FIntRect(this->rowPosition, this->columnPosition,
		this->rowPosition + this->height - 1, this->columnPosition + this->width - 1);
}