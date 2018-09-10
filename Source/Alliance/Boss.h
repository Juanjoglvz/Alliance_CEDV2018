/* Copyright (C) 2018 Ivan Garcia, Juan Jose Corroto and Javier Cordoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Boss.generated.h"

/**
 * 
 */
UCLASS()
class ALLIANCE_API ABoss : public AEnemy
{
	GENERATED_BODY()

public:
	ABoss();
	
	FORCEINLINE void setBossName(FName name) { BossName = name; }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE FName getBossName() const { return BossName; }
	FORCEINLINE void setMaxBossHealth(float maxBossHealth) { MaxBossHealth = maxBossHealth; }
	FORCEINLINE float getMaxBossHealth() const { return MaxBossHealth; }

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
		float ProgressBarHealthBinding();

protected:
	virtual void BeginPlay() override;
	
private:
	FName BossName;
	float MaxBossHealth;
	UPROPERTY()
		class USceneComponent* SceneComponent;
	UPROPERTY()
		class UWidgetComponent* WidgetComponent;
};
