/* Copyright (C) 2018 Ivan Garcia, Juan Jose Corroto and Javier Cordoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AllianceGameMode.generated.h"


UCLASS()
class AAllianceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAllianceGameMode();

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AAllianceCharacter> FirstCharacter;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AAllianceCharacter> SecondCharacter;
	UPROPERTY(EditAnywhere)
		UClass* FirstCharacterController;
	UPROPERTY(EditAnywhere)
		UClass* SecondCharacterController;

	UPROPERTY(EditAnywhere)
		TMap<TSubclassOf<class AAllianceCharacter>, UClass*> CharacterControllers;

	virtual void PostLogin(APlayerController * NewPlayer);

	UFUNCTION(Server, Reliable, WithValidation)
	void RespawnPlayer(APlayerController* SecondPlayer);
	void RespawnPlayer_Implementation(APlayerController* SecondPlayer);
	FORCEINLINE bool RespawnPlayer_Validate(APlayerController* SecondPlayer) { return true; }

private:
	AAllianceCharacter* SpawnSecondPlayerNearFirst(FVector FirstPosition, FRotator FirstRotation, TSubclassOf<class AAllianceCharacter> SecondCharacter);
	AAllianceCharacter* GetControlledPawnByAI(TArray<AActor*> CharactersInWorld);
};



