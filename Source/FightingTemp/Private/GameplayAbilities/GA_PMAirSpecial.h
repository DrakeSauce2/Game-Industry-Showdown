// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/GA_AbilityBase.h"
#include "GA_PMAirSpecial.generated.h"

/**
 * 
 */
UCLASS()
class UGA_PMAirSpecial : public UGA_AbilityBase
{
	GENERATED_BODY()
	UGA_PMAirSpecial();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<class UGameplayEffect> DamageEffect;


	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* DamageMontage;

	UFUNCTION()
	void HandleDamage(FGameplayEventData Payload);

	UFUNCTION()
	void TryCommitSpecial(FGameplayEventData Payload);

	UFUNCTION()
	void AbilityInputPressed(float TimeWaited);

	FName SpecialName;

	bool bSpecialCommitted;
	bool bIsFlipped;

	void SetupWaitInputTask();
};
