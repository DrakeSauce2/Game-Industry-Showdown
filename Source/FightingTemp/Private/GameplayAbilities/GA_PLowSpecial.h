// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/GA_AbilityBase.h"
#include "GA_PLowSpecial.generated.h"

/**
 * 
 */
UCLASS()
class UGA_PLowSpecial : public UGA_AbilityBase
{
	GENERATED_BODY()
	UGA_PLowSpecial();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<class UGameplayEffect> DamageEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Stun")
	TSubclassOf<AActor> StunSphere;

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

	void SetupWaitInputTask();

};
