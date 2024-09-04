// Fill out your copyright notice in the Description page of Project Settings.

#include "Hitbox/PushboxComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPushboxComponent::UPushboxComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Set default values
	PushForceStrength = 1000.0f;

	// Bind the overlap event
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
		// Get the character's movement component
		UCharacterMovementComponent* CharacterMovement = OtherCharacter->GetCharacterMovement();
		if (CharacterMovement)
		{
			// Calculate the push direction
			FVector PushDirection = OtherCharacter->GetActorLocation() - GetComponentLocation();
			PushDirection.Z = 0; // Keep the push horizontal
			PushDirection.Normalize();

			// Apply the push force
			FVector PushForce = PushDirection * PushForceStrength;
			CharacterMovement->AddImpulse(PushForce, true);

			// Log the push
			UE_LOG(LogTemp, Log, TEXT("Pushbox activated and pushed %s"), *OtherActor->GetName());
		}
	}
}