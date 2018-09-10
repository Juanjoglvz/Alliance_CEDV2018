/* Copyright (C) 2018 Ivan Garcia, Juan Jose Corroto and Javier Cordoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/PlayerController.h"
#include "AlliancePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ALLIANCE_API AAlliancePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay();

	UFUNCTION(Client, Reliable)
		void OnClientLogin();
	void OnClientLogin_Implementation();

	// Dialogue functions
	UFUNCTION(BlueprintCallable, Category = DlgSystem)
		void StartDialogue(class UDlgDialogue* Dialogue, UObject* FirstParticipant, UObject* SecondParticipant);
	
	UFUNCTION(BlueprintCallable, Category = DlgSystem)
		void StartDialogue3Characters(class UDlgDialogue* Dialogue, UObject* FirstParticipant, UObject* SecondParticipant, UObject* ThirdParticipant);

	UFUNCTION(BlueprintCallable, Category = DlgSystem)
		void SelectDialogueOption(int32 Index);

	FString GetPlayerNameFromController() const;

	// Character assignment
	UFUNCTION(Reliable, Server, WithValidation)
		void OnServerAssignCharacter();
	void OnServerAssignCharacter_Implementation();
	FORCEINLINE bool OnServerAssignCharacter_Validate() { return true; }

protected:

	UPROPERTY(BlueprintReadOnly)
		class UDlgContext* ActiveContext = nullptr;
	
};
