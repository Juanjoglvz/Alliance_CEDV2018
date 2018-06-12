// Fill out your copyright notice in the Description page of Project Settings.

#include "Piece.h"
#include "Engine.h"


// Sets default values
APiece::APiece() : b_IsPlayer(false), width{ 1 }, height{ 1 }
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create static mesh component
	PieceMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceMesh"));
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
	PieceMeshComponent->SetMaterial(0, NewMaterial);
}
