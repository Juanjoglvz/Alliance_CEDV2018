// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

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
		TSubclassOf<class AAllianceAIPlayerController> FirstCharacterController;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AAllianceAIPlayerController> SecondCharacterController;

	UPROPERTY(EditAnywhere)
		TMap<TSubclassOf<class AAllianceCharacter>, TSubclassOf<class AAllianceAIPlayerController>> CharacterControllers;

	virtual void PostLogin(APlayerController * NewPlayer);

	UFUNCTION(Server, Reliable, WithValidation)
	void RespawnPlayer(APlayerController* SecondPlayer);
	void RespawnPlayer_Implementation(APlayerController* SecondPlayer);
	FORCEINLINE bool RespawnPlayer_Validate(APlayerController* SecondPlayer) { return true; }

private:
	AAllianceCharacter* SpawnSecondPlayerNearFirst(FVector FirstPosition, FRotator FirstRotation, TSubclassOf<class AAllianceCharacter> SecondCharacter);
	AAllianceCharacter* GetControlledPawnByAI(TArray<AActor*> CharactersInWorld);
};



