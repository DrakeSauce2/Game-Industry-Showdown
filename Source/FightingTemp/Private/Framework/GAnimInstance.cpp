// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GAnimInstance.h"

#include "Player/GCharacterBase.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter)
	{
		OwnerCharacterBase = Cast<AGCharacterBase>(OwnerCharacter);

		OwnerMovemmentComp = OwnerCharacter->GetCharacterMovement();
	}
}

void UGAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacter && OwnerCharacterBase && OwnerMovemmentComp)
	{
		float direction = OwnerCharacterBase->GetPlayerInput().X;

		Speed = FMath::Sign(direction * OwnerCharacterBase->GetFlipDirection());

		bIsJumping = OwnerMovemmentComp->IsFalling();
	}
}
