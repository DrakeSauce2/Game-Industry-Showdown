// Fill out your copyright notice in the Description page of Project Settings.

#include "Hitbox/PushboxComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPushboxComponent::UPushboxComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	PushForceStrength = 1000.0f;

	OnComponentBeginOverlap.AddDynamic(this, &UPushboxComponent::OnPushboxBeginOverlap);
}

void UPushboxComponent::BeginPlay()
{
	Super::BeginPlay();


}

void UPushboxComponent::OnPushboxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if (OtherCharacter)
	{
		UCharacterMovementComponent* CharacterMovement = OtherCharacter->GetCharacterMovement();
		if (CharacterMovement)
		{
			FVector PushDirection = OtherCharacter->GetActorLocation() - GetComponentLocation();
			PushDirection = PushDirection.GetSafeNormal2D();

			FVector PushForce = PushDirection * PushForceStrength;
			CharacterMovement->AddImpulse(PushForce, false);
		}
	}
}