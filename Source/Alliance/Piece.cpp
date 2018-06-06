// Fill out your copyright notice in the Description page of Project Settings.

#include "Piece.h"
#include "Engine.h"


// Sets default values
APiece::APiece() : b_IsPlayer(false), width(1), height(1), Shape(TEXT("1x1"))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create static mesh component
	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceMesh"));

	auto StaticMeshAsset1x1 = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/ThirdPersonCPP/Meshes/Pieces/1x1.1x1'"));
	auto StaticMeshAsset2x1 = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/ThirdPersonCPP/Meshes/Pieces/2x1.2x1'"));
	auto StaticMeshAsset2x2 = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/ThirdPersonCPP/Meshes/Pieces/2x2.2x2'"));
	
	if (StaticMeshAsset1x1.Succeeded())
	{
		cosa1 = StaticMeshAsset1x1.Object;
	}
	if (StaticMeshAsset2x1.Succeeded())
	{
		cosa2 = StaticMeshAsset2x1.Object;
	}
	if (StaticMeshAsset2x2.Succeeded())
	{
		cosa3 = StaticMeshAsset2x2.Object;
	}
}

// Called when the game starts or when spawned
void APiece::BeginPlay()
{
	Super::BeginPlay();


	if (Shape.Equals(TEXT("1x1")))
	{
		PieceMesh->SetStaticMesh(cosa1);
	}
	else if (Shape.Equals(TEXT("2x1")))
	{
		PieceMesh->SetStaticMesh(cosa2);
	}
	else if (Shape.Equals(TEXT("2x2")))
	{
		PieceMesh->SetStaticMesh(cosa3);
	}
	PieceMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	PieceMesh->SetMaterial(0, PieceMaterial);
	PieceMesh->SetupAttachment(RootComponent);

	if (PieceMaterial)
	{
		auto DynamicMaterialInstance = UMaterialInstanceDynamic::Create(PieceMaterial, this);
		DynamicMaterialInstance->SetVectorParameterValue("Color", Color);
		SetMaterial(DynamicMaterialInstance);
	}
}
/*
// Called every frame
void APiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/

void APiece::SetMaterial(UMaterialInstanceDynamic* NewMaterial)
{
	PieceMesh->SetMaterial(0, NewMaterial);
}
