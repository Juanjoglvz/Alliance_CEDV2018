// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "AllianceCharacter.generated.h"

UCLASS(config=Game)
class AAllianceCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AAllianceCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
		void RemoveCharacterMovementBindings();

	void SetCharacterMovement(class UInputComponent* InputComponent);

	UInputComponent* GetCharacterInputComponent() { return CharacterMovementInputComponent.Get(); }

	// Variables used in blueprints for animations
	UPROPERTY(BlueprintReadWrite, Category = MyCharacter)
		bool IsRunning;
	UPROPERTY(BlueprintReadWrite, Category = MyCharacter)
		bool JumpAttacking;
	UPROPERTY(BlueprintReadWrite, Category = MyCharacter)
		bool IsAttacking;
	UPROPERTY(BlueprintReadWrite, Category = MyCharacter)
		bool ChainAttack;
	UPROPERTY(BlueprintReadWrite, Category = MyCharacter)
		float Sprint;
	UPROPERTY(BlueprintReadWrite, Category = MyCharacter)
		float LaunchForce;
	UPROPERTY(BlueprintReadWrite, Category = MyCharacter)
		float LaunchHeight;
	UPROPERTY(BlueprintReadWrite, Category = MyCharacter)
		int Combo;

private:

	TWeakObjectPtr<class UInputComponent> CharacterMovementInputComponent;

};

