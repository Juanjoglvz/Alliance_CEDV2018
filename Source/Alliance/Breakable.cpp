// Fill out your copyright notice in the Description page of Project Settings.

#include "Breakable.h"
#include "AllianceCharacter.h"
#include "Pickup.h"
#include "Engine.h"

ABreakable::ABreakable() : Super(), b_IsBroken{ false }, b_Overlaping{ false }, OverlapingCharacter { nullptr }
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create static mesh component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	
	StaticMeshComponent->SetMobility(EComponentMobility::Movable);
	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->SetIsReplicated(true);
	StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	// Create collision component
	auto BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	
	BoxCollision->bGenerateOverlapEvents = true;
	BoxCollision->SetWorldScale3D(FVector(4.f, 4.f, 4.f));
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	BoxCollision->AttachToComponent(StaticMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABreakable::OnAlyssaBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ABreakable::OnAlyssaEndOverlap);

	// Set pickup meshes
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAssetHealth(TEXT("StaticMesh'/Game/ThirdPersonCPP/Meshes/HealthPickup/heart_pickup_Sphere.heart_pickup_Sphere'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAssetStamina(TEXT("StaticMesh'/Game/ThirdPersonCPP/Meshes/StaminaPickup/StarPickup.StarPickup'"));

	Health_Asset  = MeshAssetHealth.Object;
	Stamina_Asset = MeshAssetStamina.Object;

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

void ABreakable::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (b_Overlaping && !b_IsBroken && OverlapingCharacter != nullptr)
	{
		if ((OverlapingCharacter->b_IsAttacking || OverlapingCharacter->b_JumpAttacking) && BreakableMesh_broken)
		{
			StaticMeshComponent->SetStaticMesh(BreakableMesh_broken);
			b_IsBroken = true;
			RandomDrop(OverlapingCharacter);
		}
	}
}

void ABreakable::ExecuteOnMortenHitDelegate(AAllianceCharacter* Morten)
{
	OnMortenHitDelegate.Broadcast(Morten);
}

void ABreakable::RandomDrop(AAllianceCharacter* Character)
{
	int Probability = FMath::RandRange(0, 10);

	// Spawns a pickup
	if (Probability > -1)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		APickup* Pickup = GetWorld()->SpawnActor<APickup>(	APickup::StaticClass(),
															RootComponent->GetComponentLocation() + FVector(200.f, 200.f, 75.f),
															FRotator(0.f, 0.f, 0.f), 
															SpawnInfo);

		// If health is less than a half, spawns always health pickup
		if (Pickup != nullptr)
		{
			if (Character->Health < 50.f)
			{
				Pickup->SetStaticMeshAsset(Health_Asset);
				Pickup->SetPickupType(EPickup::Health_Pickup);
			}
			// Otherwise, make a random choice between health and stamina
			else
			{
				if (FMath::RandRange(0, 10) > 6)
				{
					Pickup->SetStaticMeshAsset(Stamina_Asset);
					Pickup->SetPickupType(EPickup::Stamina_Pickup);
				}
				else
				{
					Pickup->SetStaticMeshAsset(Health_Asset);
					Pickup->SetPickupType(EPickup::Health_Pickup);
				}
			}
		}
	}
}

void ABreakable::OnAlyssaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->GetClass()->IsChildOf(AAllianceCharacter::StaticClass()))
	{
		AAllianceCharacter* Alyssa = Cast<AAllianceCharacter>(OtherActor);
		if (Alyssa)
		{
			b_Overlaping = true;
			OverlapingCharacter = Alyssa;
		}
	}
}

void ABreakable::OnAlyssaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	b_Overlaping = false;
	OverlapingCharacter = nullptr;
}


void ABreakable::OnMortenHit(AAllianceCharacter* Morten)
{
	if (BreakableMesh_broken && !b_IsBroken && Morten)
	{
		StaticMeshComponent->SetStaticMesh(BreakableMesh_broken);
		b_IsBroken = true;
		RandomDrop(Morten);
	}
}