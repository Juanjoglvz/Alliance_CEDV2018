/* Copyright (C) 2018 Iván García, Juan José Corroto and Javier Córdoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#include "Enemy.h"
#include "MeleeController.h"


// Sets default values
AEnemy::AEnemy() : CurrentState{ EEnemyState::S_Idle }, SpawnerUniqueID{ 0 }
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
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

void AEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemy, Health);
	DOREPLIFETIME(AEnemy, Damage);
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
			// Notify to spawner the enemy is dead
			OnEnemyDead.Broadcast(SpawnerUniqueID);
		}
	}
	return ActualDamage;
}
