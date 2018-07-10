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
	StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	StaticMeshComponent->SetMobility(EComponentMobility::Movable);
	StaticMeshComponent->SetSimulatePhysics(false);

	auto BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	BoxCollision->bGenerateOverlapEvents = true;
	BoxCollision->SetWorldLocation(BoxCollision->GetComponentLocation() + FVector(0.f, 0.f, 80.f));
	BoxCollision->SetWorldScale3D(FVector(2.f, 2.f, 2.f));
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	BoxCollision->AttachToComponent(StaticMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
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
				Character->IncreaseHealth(15.f);
			}
			else if (pickupType == EPickup::Stamina_Pickup)
			{
				Character->IncreaseStamina(25.f);
			}
			this->Destroy();
		}
	}
}




