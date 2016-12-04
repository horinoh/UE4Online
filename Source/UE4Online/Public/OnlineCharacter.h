// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "OnlineCharacter.generated.h"

UCLASS()
class UE4ONLINE_API AOnlineCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AOnlineCharacter(const FObjectInitializer& ObjectInitializer);

	//!< APawn
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	FORCEINLINE void StartJump() { Jump(); }
	FORCEINLINE void EndJump() { StopJumping(); }
	FORCEINLINE void StartCrouch() { Crouch(); }
	FORCEINLINE void EndCrouch() { UnCrouch(); }
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* CameraComp;
};
