// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_MeleeCombo.h"

#include "GameplayAbilities/GAbilityGenericTags.h"

#include "Animation/AnimInstance.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameplayAbilities/GAbilityGenericTags.h"
#include "GameplayTagsManager.h"

UGA_MeleeCombo::UGA_MeleeCombo()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("ability.combo.ability"));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("ability.combo.ability"));
	//ActivationBlockedTags.AddTag(UCAbilityGenericTags::GetAimingTag());

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = UGAbilityGenericTags::GetBasicAttackAcitvationTag();
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

FGameplayTag UGA_MeleeCombo::GetComboChangeTag()
{
	return FGameplayTag::RequestGameplayTag("ability.combo.change");
}

void UGA_MeleeCombo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	UAbilityTask_PlayMontageAndWait* PlayComboMotage
		= UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			ComboMontage);

	PlayComboMotage->OnBlendOut.AddDynamic(this, &UGA_MeleeCombo::K2_EndAbility);
	PlayComboMotage->OnInterrupted.AddDynamic(this, &UGA_MeleeCombo::K2_EndAbility);
	PlayComboMotage->OnCancelled.AddDynamic(this, &UGA_MeleeCombo::K2_EndAbility);
	PlayComboMotage->OnCompleted.AddDynamic(this, &UGA_MeleeCombo::K2_EndAbility);
	PlayComboMotage->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitComboEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GetComboChangeTag(), nullptr, false, false);
	WaitComboEvent->EventReceived.AddDynamic(this, &UGA_MeleeCombo::HandleComboEvent);
	WaitComboEvent->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitTargetAquiredEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UGAbilityGenericTags::GetGenericTargetAquiredTag());
	WaitTargetAquiredEvent->EventReceived.AddDynamic(this, &UGA_MeleeCombo::HandleDamage);
	WaitTargetAquiredEvent->ReadyForActivation();

	SetupWaitInputTask();

	//needed for ai to tirgger basic attack.
	UAbilityTask_WaitGameplayEvent* WaitForActivation = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UGAbilityGenericTags::GetBasicAttackAcitvationTag());
	WaitForActivation->EventReceived.AddDynamic(this, &UGA_MeleeCombo::TryCommitCombo);
	WaitForActivation->ReadyForActivation();
}

void UGA_MeleeCombo::HandleComboEvent(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("Handling Event!"));

	FGameplayTag ComboTag = Payload.EventTag;
	if (ComboTag == FGameplayTag::RequestGameplayTag("ability.combo.change.end"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Combo End!"));
		NextComboName = NAME_None;
		bComboCommitted = false;
		return;
	}

	TArray<FName> ComboNames;
	UGameplayTagsManager::Get().SplitGameplayTagFName(ComboTag, ComboNames);
	NextComboName = ComboNames.Last();
}

void UGA_MeleeCombo::HandleDamage(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("Handling Damage!"));

	FGameplayEffectSpecHandle EffectSpec = MakeOutgoingGameplayEffectSpec(GetDamageEffectForCurrentCombo(), GetAbilityLevel(CurrentSpecHandle, CurrentActorInfo));
	ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpec, Payload.TargetData);
}

void UGA_MeleeCombo::TryCommitCombo(FGameplayEventData Payload)
{
	if (bComboCommitted)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combo Already Commited"));

		return;
	}

	if (NextComboName == NAME_None)
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

	OwnerAnimInst->Montage_SetNextSection(OwnerAnimInst->Montage_GetCurrentSection(ComboMontage), NextComboName, ComboMontage);
	bComboCommitted = true;
}

const TSubclassOf<UGameplayEffect> UGA_MeleeCombo::GetDamageEffectForCurrentCombo() const
{
	const USkeletalMeshComponent* OwnerMesh = GetOwningComponentFromActorInfo();
	if (!OwnerMesh)
		return TSubclassOf<UGameplayEffect>();

	const UAnimInstance* OwnerAnimInst = OwnerMesh->GetAnimInstance();
	if (!OwnerAnimInst)
		return TSubclassOf<UGameplayEffect>();

	const FName SectionName = OwnerAnimInst->Montage_GetCurrentSection(ComboMontage);

	const TSubclassOf<UGameplayEffect>* FoundEffect = ComboDamageEffectMap.Find(SectionName);
	if (FoundEffect)
	{
		return *FoundEffect;
	}

	return TSubclassOf<UGameplayEffect>();
}

void UGA_MeleeCombo::AbilityInputPressed(float TimeWaited)
{
	SetupWaitInputTask();
	TryCommitCombo(FGameplayEventData());
}

void UGA_MeleeCombo::SetupWaitInputTask()
{
	UAbilityTask_WaitInputPress* WaitInputPress = UAbilityTask_WaitInputPress::WaitInputPress(this);
	WaitInputPress->OnPress.AddDynamic(this, &UGA_MeleeCombo::AbilityInputPressed);
	WaitInputPress->ReadyForActivation();
}
