// Fill out your copyright notice in the Description page of Project Settings.

#include "Fireball.h"
#include "Enemy.h"


// Sets default values
AFireball::AFireball() : TTL{ 6.f }, LivingTime{ 0.f }
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	RootComponent = StaticMeshComponent;

	PMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");

	// Load assets
	auto StaticMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/ThirdPersonCPP/Meshes/Pieces/1x1.1x1'"));

	if (StaticMeshAsset.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(StaticMeshAsset.Object);
	}

	// Set properties for components
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetEnableGravity(false);
	PMovementComponent->InitialSpeed = 600.f;
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	StaticMeshComponent->SetNotifyRigidBodyCollision(true);
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AFireball::OnHit);
	

}


// Called every frame
void AFireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Increase Living time
	LivingTime += DeltaTime;

	// Destroy if lived more than enough
	if (LivingTime > TTL)
	{
		this->Destroy();
	}

	// Move in every frame
	StaticMeshComponent->AddImpulseAtLocation(GetVelocity(), GetActorLocation());
}

void AFireball::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);

	if (Enemy != nullptr)
	{
		UGameplayStatics::ApplyDamage(Enemy, 50.f, nullptr, nullptr, nullptr);
	}

	this->Destroy();

}
