/* Copyright (C) 2018 Ivan Garcia, Juan Jose Corroto and Javier Cordoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#pragma once

#include "Core.h"
#include "Engine/GameInstance.h"
#include "AllianceCharacter.h"
#include "AllianceGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALLIANCE_API UAllianceGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UAllianceGameInstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		TMap<FString, TSubclassOf<AAllianceCharacter>> AssignedCharacters;
	
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

	UFUNCTION(BlueprintCallable, Category = "Online")
		bool isSteamActivated(class APlayerController* PlayerController);
	
};
