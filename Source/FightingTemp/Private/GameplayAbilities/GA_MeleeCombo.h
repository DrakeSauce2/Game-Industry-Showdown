// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/GA_AbilityBase.h"
#include "GA_MeleeCombo.generated.h"

/**
 * 
 */
UCLASS()
class UGA_MeleeCombo : public UGA_AbilityBase
{
	GENERATED_BODY()
	
public:
	UGA_MeleeCombo();
	static FGameplayTag GetComboChangeTag();

private:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* ComboMontage;

	UFUNCTION()
	void HandleComboEvent(FGameplayEventData Payload);

	UFUNCTION()
	void HandleDamage(FGameplayEventData Payload);

	UFUNCTION()
	void TryCommitCombo(FGameplayEventData Payload);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FName, TSubclassOf<class UGameplayEffect>> ComboDamageEffectMap;

	const TSubclassOf<UGameplayEffect> GetDamageEffectForCurrentCombo() const;

	UFUNCTION()
	void AbilityInputPressed(float TimeWaited);

	FName NextComboName;

	bool bComboCommitted;

	void SetupWaitInputTask();

};
