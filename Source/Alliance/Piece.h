// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/Color.h"
#include "GameFramework/Actor.h"
#include "Piece.generated.h"

UCLASS()
class ALLIANCE_API APiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APiece();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

	void SetMaterial(class UMaterialInstanceDynamic* NewMaterial);

	UPROPERTY(EditAnywhere)
		bool b_IsPlayer;
	UPROPERTY(EditAnywhere)
		int width;
	UPROPERTY(EditAnywhere)
		int height;
	UPROPERTY(EditAnywhere)
		int rowPosition;
	UPROPERTY(EditAnywhere)
		int columnPosition;
	
	UPROPERTY(EditAnywhere)
		class UMaterialInstance* PieceMaterial;
	UPROPERTY(EditAnywhere)
		class UStaticMesh* PieceMesh;

	UPROPERTY(EditAnywhere)
		FLinearColor Color;

private:
	
	UPROPERTY()
		UStaticMeshComponent* PieceMeshComponent;

};
