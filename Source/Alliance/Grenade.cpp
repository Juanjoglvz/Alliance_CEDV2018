/* Copyright (C) 2018 Iván García, Juan José Corroto and Javier Córdoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

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
	
	PMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");

	PSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("Particle System");

	// Load assets
	auto StaticMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/ThirdPersonCPP/Meshes/Morten_Grenade.Morten_Grenade'"));

	if (StaticMeshAsset.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(StaticMeshAsset.Object);
	}

	auto PSystemAsset = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/ThirdPersonCPP/Meshes/FX/P_GrenadeDirt.P_GrenadeDirt'"));

	if (PSystemAsset.Succeeded())
	{
		PSystemComponent->SetTemplate(PSystemAsset.Object);
	}

	// Set properties for components
	StaticMeshComponent->SetSimulatePhysics(true);
	PMovementComponent->InitialSpeed = 750.f;
	PMovementComponent->bShouldBounce = false;
	PMovementComponent->Bounciness = 0.02f;
	PMovementComponent->Friction = 1.f;

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

	// Spawn Emitter and make sound
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PSystemComponent->Template, GetActorTransform());

	// Destroy Actor
	this->Destroy();
}
