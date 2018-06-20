// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AllianceGameMode.generated.h"

UCLASS(minimalapi)
class AAllianceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAllianceGameMode();
	virtual void PostLogin(APlayerController * NewPlayer);
};



