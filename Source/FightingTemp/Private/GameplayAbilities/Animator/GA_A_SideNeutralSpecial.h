// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/GA_AbilityBase.h"
#include "GA_A_SideNeutralSpecial.generated.h"

/**
 * 
 */
UCLASS()
class UGA_A_SideNeutralSpecial : public UGA_AbilityBase
{
	GENERATED_BODY()

	UGA_A_SideNeutralSpecial();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* DamageMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Actor")
	TSubclassOf<AActor> KeyFrameClass;
};
