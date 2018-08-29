/* Copyright (C) 2018 Iván García, Juan José Corroto and Javier Córdoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#include "EnemyFactory.h"
#include "Engine.h"
#include "Enemy.h"

// Sets default values
AEnemyFactory::AEnemyFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	// Get references to enemies blueprints
	static ConstructorHelpers::FObjectFinder<UBlueprint> MeleeBP(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/Enemies/MeleeEnemy.MeleeEnemy'"));
	if (MeleeBP.Object) {
		EnemyMeleeReference = (UClass*)MeleeBP.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> RangedBP(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/Enemies/RangedEnemy.RangedEnemy'"));
	if (RangedBP.Object) {
		EnemyRangedReference = (UClass*)RangedBP.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> TankBP(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/Enemies/TankEnemy.TankEnemy'"));
	if (TankBP.Object) {
		EnemyTankReference = (UClass*)TankBP.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> HenkkaBP(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/Enemies/Henkka.Henkka'"));
	if (HenkkaBP.Object) {
		EnemyHenkkaReference = (UClass*)HenkkaBP.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> ShivaBP(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/Enemies/Shiva.Shiva'"));
	if (ShivaBP.Object) {
		EnemyShivaReference = (UClass*)ShivaBP.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> EuronymousBP(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/Enemies/Euronymous.Euronymous'"));
	if (EuronymousBP.Object) {
		EnemyEuronymousReference = (UClass*)EuronymousBP.Object->GeneratedClass;
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
		}
		else if (EnemyType == EEnemyType::ET_Shiva)
		{
			EnemyToReturn = GetWorld()->SpawnActor<AEnemy>(EnemyShivaReference, Location, Rotation, SpawnParams);
		}
		else if (EnemyType == EEnemyType::ET_Henkka)
		{
			EnemyToReturn = GetWorld()->SpawnActor<AEnemy>(EnemyHenkkaReference, Location, Rotation, SpawnParams);
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


