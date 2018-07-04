// Fill out your copyright notice in the Description page of Project Settings.

#include "Grenade.h"
#include "AllianceCharacter.h"


// Sets default values
AGrenade::AGrenade() : Dmg{ 50 }, Timer{ 4.f }
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create components
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	RootComponent = StaticMeshComponent;
	
	PMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement Component");

	auto StaticMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/ThirdPersonCPP/Meshes/Morten_Grenade.Morten_Grenade'"));

	if (StaticMeshAsset.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(StaticMeshAsset.Object);
	}

	// Set properties for components
	StaticMeshComponent->SetSimulatePhysics(true);
	PMovementComponent->InitialSpeed = 1000.f;
	PMovementComponent->bShouldBounce = true;
	PMovementComponent->Bounciness = 0.1f;
	PMovementComponent->Friction = 0.9f;

}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();

	// Add impulse when spawned
	FVector Impulse = GetVelocity() * FVector { 20.f, 20.f, 20.f };
	StaticMeshComponent->AddImpulseAtLocation(Impulse, GetActorLocation());

	// Set Timer for explosion
	FTimerHandle UnusedHandler;
	GetWorldTimerManager().SetTimer(UnusedHandler, this, &AGrenade::Explode, Timer, false);
	
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrenade::Explode()
{
	//Get Players so that they ignore damage
	TArray<AActor*> IgnoredActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAllianceCharacter::StaticClass(), IgnoredActors);

	// Deal Damage
	UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), Dmg, Dmg / 5, GetActorLocation(), 300.f, 500.f, 1.f, nullptr, IgnoredActors, this, nullptr);

	// Destroy Actor
	this->Destroy();
}
