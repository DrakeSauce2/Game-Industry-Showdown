// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/Animator/GA_ADownSpecial.h"
#include "GameplayAbilities/GAbilityGenericTags.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UGA_ADownSpecial::UGA_ADownSpecial()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("ability.combo.ability"));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("ability.combo.ability"));

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = UGAbilityGenericTags::GetBasicAttackAcitvationTag();
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UGA_ADownSpecial::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("Ability Activated!"));

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed To Commit Ability!"));

		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Ability Commited! Playing Montage! WOrks!!!!!"));

	FActorSpawnParameters SpawnParams;
	FVector Location = FVector(100.0f, 100.0f, 300.0f); // Location to spawn
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);      // Rotation
	AActor* TempActor = GetWorld()->SpawnActor<AActor>(KeyFrameClass, Location, Rotation, SpawnParams);

	if (TempActor)
	{

	}
}
