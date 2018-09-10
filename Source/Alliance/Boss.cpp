/* Copyright (C) 2018 Ivan Garcia, Juan Jose Corroto and Javier Cordoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#include "Boss.h"
#include "Engine.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

ABoss::ABoss() : Super(), BossName("Nothing"), MaxBossHealth(100.f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Create SceneComponent
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Create WidgetComponent
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetAsset (TEXT("/Game/ThirdPersonCPP/Blueprints/Widgets/EnemiesHealthWidget"));
	if (WidgetAsset.Succeeded())
	{
		WidgetComponent->SetWidgetClass(WidgetAsset.Class);
	}
	WidgetComponent->SetWorldLocation(WidgetComponent->GetComponentLocation() + FVector(0.f, 100.f, 50.f));
	WidgetComponent->SetWorldScale3D(FVector(0.3f));
	WidgetComponent->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

void ABoss::BeginPlay()
{
	Super::BeginPlay();
}

void ABoss::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(SceneComponent->GetComponentLocation(), 
														      UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
	SceneComponent->SetWorldRotation(Rotator);
}

float ABoss::ProgressBarHealthBinding()
{
	return Health / MaxBossHealth;
}
