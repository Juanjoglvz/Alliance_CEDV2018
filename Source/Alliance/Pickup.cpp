// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "Engine.h"
#include "AllianceCharacter.h"


// Sets default values
APickup::APickup() 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	auto BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	BoxCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->bGenerateOverlapEvents = true;
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlap);

	// By default will be health pickup
	pickupType = EPickup::Health_Pickup;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickup::SetStaticMeshAsset(UStaticMesh* StaticMeshAsset)
{
	if (StaticMeshAsset != nullptr)
	{
		StaticMeshComponent->SetStaticMesh(StaticMeshAsset);
	}
	StaticMeshComponent->SetMobility(EComponentMobility::Movable);
	StaticMeshComponent->SetEnableGravity(false);
	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void APickup::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (OtherActor->GetClass()->IsChildOf(AAllianceCharacter::StaticClass()))
		{
			AAllianceCharacter* Character = (AAllianceCharacter*)OtherActor;
			if (pickupType == EPickup::Health_Pickup)
			{
				// Character->IncreaseHealth;
			}
			else if (pickupType == EPickup::Health_Pickup)
			{
				// Character->IncreaseStamina;
			}
			this->Destroy();
		}
	}
}




