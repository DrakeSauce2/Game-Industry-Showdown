// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_PNeutralSpecial.h"

#include "GameplayAbilities/GAbilityGenericTags.h"

#include "Animation/AnimInstance.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameplayAbilities/GAbilityGenericTags.h"
#include "GameplayTagsManager.h"

UGA_PNeutralSpecial::UGA_PNeutralSpecial()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("ability.combo.ability"));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("ability.combo.ability"));

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = UGAbilityGenericTags::GetBasicAttackAcitvationTag();
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);

	DialogueSpawn = nullptr;
}

void UGA_PNeutralSpecial::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("Ability Activated!"));

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed To Commit Ability!"));

		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Ability Commited! Playing Montage!"));

	if (DialogueSpawn)
	{
		FVector SpawnLocation = ActorInfo->OwnerActor->GetActorLocation() + FVector(200.0f, 0.0f, 100.0f);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = ActorInfo->OwnerActor.Get();
		//SpawnParams.Instigator = ActorInfo->AvatarActor.Get();
		//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(DialogueSpawn, SpawnLocation, SpawnRotation, SpawnParams);

		if (SpawnedActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawned Object: %s"), *SpawnedActor->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Faild to spawn object."));
		}

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No class set to spawn."));
	}

	UAbilityTask_PlayMontageAndWait* PlayComboMotage
		= UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			DamageMontage);

	PlayComboMotage->OnBlendOut.AddDynamic(this, &UGA_PNeutralSpecial::K2_EndAbility);
	PlayComboMotage->OnInterrupted.AddDynamic(this, &UGA_PNeutralSpecial::K2_EndAbility);
	PlayComboMotage->OnCancelled.AddDynamic(this, &UGA_PNeutralSpecial::K2_EndAbility);
	PlayComboMotage->OnCompleted.AddDynamic(this, &UGA_PNeutralSpecial::K2_EndAbility);
	PlayComboMotage->ReadyForActivation();

	//UAbilityTask_WaitGameplayEvent* WaitComboEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GetComboChangeTag(), nullptr, false, false);
	//WaitComboEvent->EventReceived.AddDynamic(this, &UGA_MeleeCombo::HandleComboEvent);
	//WaitComboEvent->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitTargetAquiredEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UGAbilityGenericTags::GetGenericTargetAquiredTag());
	WaitTargetAquiredEvent->EventReceived.AddDynamic(this, &UGA_PNeutralSpecial::HandleDamage);
	WaitTargetAquiredEvent->ReadyForActivation();

	SetupWaitInputTask();

	//needed for ai to tirgger basic attack.
	UAbilityTask_WaitGameplayEvent* WaitForActivation = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UGAbilityGenericTags::GetBasicAttackAcitvationTag());
	WaitForActivation->EventReceived.AddDynamic(this, &UGA_PNeutralSpecial::TryCommitSpecial);

	WaitForActivation->ReadyForActivation();
}

void UGA_PNeutralSpecial::HandleDamage(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("Handling Damage!"));

	FGameplayEffectSpecHandle EffectSpec = MakeOutgoingGameplayEffectSpec(DamageEffect, GetAbilityLevel(CurrentSpecHandle, CurrentActorInfo));
	ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpec, Payload.TargetData);
}

void UGA_PNeutralSpecial::TryCommitSpecial(FGameplayEventData Payload)
{
	if (bSpecialCommitted)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combo Already Commited"));

		return;
	}

	if (SpecialName == NAME_None)
	{
		return;
	}

	USkeletalMeshComponent* OwnerMesh = GetOwningComponentFromActorInfo();
	if (!OwnerMesh)
	{
		return;
	}

	UAnimInstance* OwnerAnimInst = OwnerMesh->GetAnimInstance();
	if (!OwnerAnimInst)
	{
		return;
	}

	OwnerAnimInst->Montage_SetNextSection(OwnerAnimInst->Montage_GetCurrentSection(DamageMontage), SpecialName, DamageMontage);
	bSpecialCommitted = true;
}

void UGA_PNeutralSpecial::AbilityInputPressed(float TimeWaited)
{
	SetupWaitInputTask();
	TryCommitSpecial(FGameplayEventData());
}

void UGA_PNeutralSpecial::SetupWaitInputTask()
{
	UAbilityTask_WaitInputPress* WaitInputPress = UAbilityTask_WaitInputPress::WaitInputPress(this);
	WaitInputPress->OnPress.AddDynamic(this, &UGA_PNeutralSpecial::AbilityInputPressed);
	WaitInputPress->ReadyForActivation();
}
