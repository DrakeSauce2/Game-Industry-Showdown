// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_PHighSpecial.h"

#include "GameplayAbilities/GAbilityGenericTags.h"

#include "Animation/AnimInstance.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameplayAbilities/GAbilityGenericTags.h"
#include "GameplayTagsManager.h"

#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"

UGA_PHighSpecial::UGA_PHighSpecial()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("ability.combo.ability"));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("ability.combo.ability"));

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = UGAbilityGenericTags::GetBasicAttackAcitvationTag();
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UGA_PHighSpecial::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	//Play Ability Here
	int32 RandomInt = NumberGenerator(1, 4);
	UE_LOG(LogTemp, Warning, TEXT("Random Number: %d"), RandomInt);
	switch (RandomInt)
	{
	case 1:
		UE_LOG(LogTemp, Warning, TEXT("Upgrade one: Gain Health"));
		break;
	case 2:
		UE_LOG(LogTemp, Warning, TEXT("Upgrade one: Gain Speed"));
		break;
	case 3:
		UE_LOG(LogTemp, Warning, TEXT("Upgrade one: Gain Knockback"));
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Upgrade Not Found."));
		break;
	}

	//StartTimer();

	UAbilityTask_PlayMontageAndWait* PlayComboMotage
		= UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			DamageMontage);

	PlayComboMotage->OnBlendOut.AddDynamic(this, &UGA_PHighSpecial::K2_EndAbility);
	PlayComboMotage->OnInterrupted.AddDynamic(this, &UGA_PHighSpecial::K2_EndAbility);
	PlayComboMotage->OnCancelled.AddDynamic(this, &UGA_PHighSpecial::K2_EndAbility);
	PlayComboMotage->OnCompleted.AddDynamic(this, &UGA_PHighSpecial::K2_EndAbility);
	PlayComboMotage->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitTargetAquiredEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UGAbilityGenericTags::GetGenericTargetAquiredTag());
	WaitTargetAquiredEvent->EventReceived.AddDynamic(this, &UGA_PHighSpecial::HandleDamage);
	WaitTargetAquiredEvent->ReadyForActivation();

	SetupWaitInputTask();

	//needed for ai to tirgger basic attack.
	UAbilityTask_WaitGameplayEvent* WaitForActivation = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UGAbilityGenericTags::GetBasicAttackAcitvationTag());
	WaitForActivation->EventReceived.AddDynamic(this, &UGA_PHighSpecial::TryCommitSpecial);
	WaitForActivation->ReadyForActivation();
}

void UGA_PHighSpecial::HandleDamage(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("Handling Damage!"));

	FGameplayEffectSpecHandle EffectSpec = MakeOutgoingGameplayEffectSpec(DamageEffect, GetAbilityLevel(CurrentSpecHandle, CurrentActorInfo));
	ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpec, Payload.TargetData);
}

void UGA_PHighSpecial::TryCommitSpecial(FGameplayEventData Payload)
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

void UGA_PHighSpecial::AbilityInputPressed(float TimeWaited)
{
	SetupWaitInputTask();
	TryCommitSpecial(FGameplayEventData());
}

int32 UGA_PHighSpecial::NumberGenerator(int32 Min, int32 Max)
{
	return FMath::RandRange(Min,Max);
}

void UGA_PHighSpecial::SetupWaitInputTask()
{
	UAbilityTask_WaitInputPress* WaitInputPress = UAbilityTask_WaitInputPress::WaitInputPress(this);
	WaitInputPress->OnPress.AddDynamic(this, &UGA_PHighSpecial::AbilityInputPressed);
	WaitInputPress->ReadyForActivation();
}

//void UGA_PHighSpecial::StartTimer()
//{
//	UE_LOG(LogTemp, Warning, TEXT("I have started the time"));
//	GetWorld()->GetTimerManager().SetTimer(BoostTimerHandle, this, &UGA_PHighSpecial::EndBoost, BoostTime, false);
//}
//
//void UGA_PHighSpecial::EndBoost()
//{
//	UE_LOG(LogTemp, Error, TEXT("I have ended the time"));
//	//End the boosted ability
//}

