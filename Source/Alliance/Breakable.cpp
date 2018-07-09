// Fill out your copyright notice in the Description page of Project Settings.

#include "Breakable.h"
#include "AllianceCharacter.h"
#include "Pickup.h"
#include "Engine.h"

ABreakable::ABreakable() : Super(), b_IsBroken{ false }
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create static mesh component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	
	StaticMeshComponent->SetMobility(EComponentMobility::Movable);
	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->SetIsReplicated(true);
	//StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	// Create collision component
	auto BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->bGenerateOverlapEvents = true;
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	BoxCollision->AttachToComponent(StaticMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABreakable::OnAlyssaHit);

	// Set pickup meshes
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/ThirdPersonCPP/Meshes/Morten_Grenade.Morten_Grenade'"));
	Health_Asset = MeshAsset.Object;
	Stamina_Asset = MeshAsset.Object;

	OnMortenHitDelegate.AddDynamic(this, &ABreakable::OnMortenHit);
}

void ABreakable::BeginPlay()
{
	Super::BeginPlay();

	if (BreakableMesh)
	{
		StaticMeshComponent->SetStaticMesh(BreakableMesh);
	}
}

void ABreakable::ExecuteOnMortenHitDelegate()
{
	OnMortenHitDelegate.Broadcast();
}

void ABreakable::RandomDrop()
{
	int Probability = FMath::RandRange(0, 10);

	// Spawns a pickup
	if (Probability > -1)
	{
		FActorSpawnParameters SpawnInfo;
		APickup* Pickup = GetWorld()->SpawnActor<APickup>(GetActorLocation(), GetActorRotation(), SpawnInfo);
		
		Pickup->SetStaticMeshAsset(Stamina_Asset);
		Pickup->SetPickupType(EPickup::Stamina_Pickup);
	}
}

void ABreakable::OnAlyssaHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->GetClass()->IsChildOf(AAllianceCharacter::StaticClass()))
	{
		AAllianceCharacter* Alyssa = Cast<AAllianceCharacter>(OtherActor);
		if (Alyssa)
		{
			if ((Alyssa->b_IsAttacking || Alyssa->b_JumpAttacking) && BreakableMesh_broken && !b_IsBroken)
			{
				StaticMeshComponent->SetStaticMesh(BreakableMesh_broken);
				b_IsBroken = true;
				RandomDrop();
			}
		}
	}
}

void ABreakable::OnMortenHit()
{
	if (BreakableMesh_broken && !b_IsBroken)
	{
		StaticMeshComponent->SetStaticMesh(BreakableMesh_broken);
		b_IsBroken = true;
		RandomDrop();
	}
}