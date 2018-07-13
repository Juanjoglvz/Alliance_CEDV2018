// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "MeleeController.h"


// Sets default values
AEnemy::AEnemy() : CurrentState{EEnemyState::S_Idle}
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::DoDmg(AActor* DamagedActor)
{
	UGameplayStatics::ApplyDamage(DamagedActor, Damage, nullptr, nullptr, nullptr);
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;
		if (Health <= 0)
		{
			// The Enemy is dead
			this->Destroy();
		}

		UE_LOG(LogTemp, Error, TEXT("Hit the enmy: \t Health: %f \t"), Health);
	}
	return ActualDamage;
}
