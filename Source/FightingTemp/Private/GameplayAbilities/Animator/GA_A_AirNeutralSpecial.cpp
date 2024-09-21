// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/Animator/GA_A_AirNeutralSpecial.h"
#include "GameplayAbilities/GAbilityGenericTags.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Player/GFighterCharacter.h"


UGA_A_AirNeutralSpecial::UGA_A_AirNeutralSpecial()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("ability.combo.ability"));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("ability.combo.ability"));

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = UGAbilityGenericTags::GetBasicAttackAcitvationTag();
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
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
		AGFighterCharacter* PlayerCharacter = Cast<AGFighterCharacter>(PlayerActor);
		if (PlayerCharacter)
		{
			// Get the saved location
			FVector SavedLocation = PlayerCharacter->GetSavedKeyFrameActorLocation();

			// Teleport or move the player to the saved location
			PlayerCharacter->SetActorLocation(SavedLocation);

			UE_LOG(LogTemp, Warning, TEXT("Player teleported to saved location!"));
		}
	}

	// End the ability manually so it can be reactivated later
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
