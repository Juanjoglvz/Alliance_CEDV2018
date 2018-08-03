// Fill out your copyright notice in the Description page of Project Settings.

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
		void StartDialogue(class UDlgDialogue* Dialogue, UObject* OtherParticipant);

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
