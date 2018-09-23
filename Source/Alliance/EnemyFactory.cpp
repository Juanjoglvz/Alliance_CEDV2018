/* Copyright (C) 2018 Ivan Garcia, Juan Jose Corroto and Javier Cordoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#include "EnemyFactory.h"
#include "Engine.h"
#include "Enemy.h"
#include "Boss.h"

// Sets default values
AEnemyFactory::AEnemyFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	// Get references to enemies blueprints
	static ConstructorHelpers::FClassFinder<APawn> MeleeBP(TEXT("/Game/ThirdPersonCPP/Blueprints/Enemies/MeleeEnemy"));
	if (MeleeBP.Class) {
		EnemyMeleeReference = MeleeBP.Class;
	}

	static ConstructorHelpers::FClassFinder<APawn> RangedBP(TEXT("/Game/ThirdPersonCPP/Blueprints/Enemies/RangedEnemy"));
	if (RangedBP.Class) {
		EnemyRangedReference = RangedBP.Class;
	}

	static ConstructorHelpers::FClassFinder<APawn> TankBP(TEXT("/Game/ThirdPersonCPP/Blueprints/Enemies/TankEnemy"));
	if (TankBP.Class) {
		EnemyTankReference = TankBP.Class;
	}

	static ConstructorHelpers::FClassFinder<APawn> HenkkaBP(TEXT("/Game/ThirdPersonCPP/Blueprints/Enemies/Henkka"));
	if (HenkkaBP.Class) {
		EnemyHenkkaReference = HenkkaBP.Class;
	}

	static ConstructorHelpers::FClassFinder<APawn> ShivaBP(TEXT("/Game/ThirdPersonCPP/Blueprints/Enemies/Shiva"));
	if (ShivaBP.Class) {
		EnemyShivaReference = ShivaBP.Class;
	}

	static ConstructorHelpers::FClassFinder<APawn> EuronymousBP(TEXT("/Game/ThirdPersonCPP/Blueprints/Enemies/Euronymous"));
	if (EuronymousBP.Class) {
		EnemyEuronymousReference = EuronymousBP.Class;
	}
}

// Called when the game starts or when spawned
void AEnemyFactory::BeginPlay()
{
	Super::BeginPlay();
	
	Instigator = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	SetOwner(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AEnemyFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AEnemy* AEnemyFactory::SpawnAnEnemy(EEnemyType EnemyType, FVector Location, FRotator Rotation, uint32 SpawnerUniqueID)
{
	AEnemy* EnemyToReturn;
	if (GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (EnemyType == EEnemyType::ET_Melee)
		{
			EnemyToReturn = GetWorld()->SpawnActor<AEnemy>(EnemyMeleeReference, Location, Rotation, SpawnParams);
		}
		else if (EnemyType == EEnemyType::ET_Ranged)
		{
			EnemyToReturn = GetWorld()->SpawnActor<AEnemy>(EnemyRangedReference, Location, Rotation, SpawnParams);
		}
		else if (EnemyType == EEnemyType::ET_Tank)
		{
			EnemyToReturn = GetWorld()->SpawnActor<AEnemy>(EnemyTankReference, Location, Rotation, SpawnParams);
		} 
		else if (EnemyType == EEnemyType::ET_Euronymous)
		{
			EnemyToReturn = GetWorld()->SpawnActor<AEnemy>(EnemyEuronymousReference, Location, Rotation, SpawnParams);
			ABoss* Boss = Cast<ABoss>(EnemyToReturn);
			Boss->setBossName(FName("Euronymous")); 
			Boss->setMaxBossHealth(800.f);
		}
		else if (EnemyType == EEnemyType::ET_Shiva)
		{
			EnemyToReturn = GetWorld()->SpawnActor<AEnemy>(EnemyShivaReference, Location, Rotation, SpawnParams);
			ABoss* Boss = Cast<ABoss>(EnemyToReturn);
			Boss->setBossName(FName("Shiva"));
			Boss->setMaxBossHealth(1200.f);
		}
		else if (EnemyType == EEnemyType::ET_Henkka)
		{
			EnemyToReturn = GetWorld()->SpawnActor<AEnemy>(EnemyHenkkaReference, Location, Rotation, SpawnParams);
			ABoss* Boss = Cast<ABoss>(EnemyToReturn);
			Boss->setBossName(FName("Henkka"));
			Boss->setMaxBossHealth(1000.f);
		}
		else
		{
			EnemyToReturn = nullptr;
		}
	}
	else
	{
		EnemyToReturn = nullptr;
	}

	if (EnemyToReturn)
	{
		EnemyToReturn->SpawnerUniqueID = SpawnerUniqueID;
	}

	return EnemyToReturn;
}


