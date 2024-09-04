// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GAnimInstance.generated.h"

class ACharacter;
class AGCharacterBase;
class UCharacterMovementComponent;
class AFightingTempCharacter;
/**
 * 
 */
UCLASS()
class UGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsFlipped;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsJumping;

private:
	// the below functions are the native overrides for each phase
	// Native initialization override point BEGIN PLAY
	virtual void NativeInitializeAnimation() override;

	// Native thread safe update override point. Executed on a worker thread just prior to graph update 
	// for linked anim instances, only called when the hosting node(s) are relevant TICK
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	const ACharacter* OwnerCharacter;
	const AGCharacterBase* OwnerCharacterBase; // For Testing
	const UCharacterMovementComponent* OwnerMovemmentComp;

};
