// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/Animator/GA_A_AirNeutralSpecial.h"
#include "GameplayAbilities/GAbilityGenericTags.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Player/GFighterCharacter.h"
#include "TimerManager.h"


UGA_A_AirNeutralSpecial::UGA_A_AirNeutralSpecial()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("ability.combo.ability"));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("ability.combo.ability"));

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = UGAbilityGenericTags::GetBasicAttackAcitvationTag();
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
	
	SpeedOfAttackMovement = 300.0f;

}

void UGA_A_AirNeutralSpecial::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("Ability Activated!"));

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed To Commit Ability!"));

		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Ability Commited! AIR NEUTRAL WOrks!!!!!"));

	if (ActorInfo && ActorInfo->AvatarActor.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Avatar Actor is Valid, Yuhhhhhh!!!!!"));

		//ActorInfo->OwnerActor.Get()
		AActor* PlayerActor = ActorInfo->AvatarActor.Get();

		UE_LOG(LogTemp, Warning, TEXT("Actor spawned at player's location!"));

		// Save the location of the spawned actor
		PlayerCharacter = Cast<AGFighterCharacter>(PlayerActor);
		if (PlayerCharacter)
		{
			// Set up the timer and delegate for interpolation
			FTimerHandle TimerHandle;
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUFunction(this, FName("UpdateCharacterLocation"));

			// Start the timer to call the Interp function every frame (or a suitable interval)
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.01f, true); // Every 0.01 seconds

			UE_LOG(LogTemp, Warning, TEXT("Timer started for interpolation"));

		}
	}

	// End the ability manually so it can be reactivated later
	//EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}


void UGA_A_AirNeutralSpecial::UpdateCharacterLocation()
{

	float InterpSpeed = 5.0f;  // Adjust speed as needed
	FVector CurrentLocation = PlayerCharacter->GetActorLocation();

	// Interpolate towards the target location
	FVector NewLocation = FMath::VInterpTo(CurrentLocation, PlayerCharacter->GetSavedKeyFrameActorLocation(), GetWorld()->GetDeltaSeconds(), InterpSpeed);

	// Move the character to the new location
	PlayerCharacter->SetActorLocation(NewLocation);

	// Check if the character is close enough to stop the interpolation
	if (FVector::Dist(NewLocation, PlayerCharacter->GetSavedKeyFrameActorLocation()) < 50.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character reached target location, stopping timer"));

		// Stop the timer when we reach the target
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

		// end the ability
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}

}

void UGA_A_AirNeutralSpecial::StopCharacterMovement()
{

}