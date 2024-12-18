// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/GA_AbilityBase.h"
#include "GA_PHighSpecial.generated.h"

/**
 * 
 */
UCLASS()
class UGA_PHighSpecial : public UGA_AbilityBase
{
	GENERATED_BODY()
	UGA_PHighSpecial();

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

	UFUNCTION(BlueprintCallable, Category = "Random")
	int32 NumberGenerator(int32 Min, int32 Max);


	FName SpecialName;

	bool bSpecialCommitted;

	void SetupWaitInputTask();

	///*	Timer	*/
	//FTimerHandle BoostTimerHandle;
	//
	//UPROPERTY(EditDefaultsOnly, Category = "Timer")
	//float BoostTime;

	//UFUNCTION()
	//void StartTimer();

	//UFUNCTION()
	//void EndBoost();
};
