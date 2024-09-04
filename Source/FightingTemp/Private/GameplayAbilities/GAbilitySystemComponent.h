// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilities/GAbilitySystemTypes.h"
#include "GAbilitySystemComponent.generated.h"

USTRUCT(BlueprintType)
struct FDirectionAttackKey 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAbilityInputID InputType;

	bool operator==(const FDirectionAttackKey& Other) const 
	{
		return Direction.Equals(Other.Direction, 0.5f) && InputType == Other.InputType;
	}

	// Hash Function for TMap
	friend uint32 GetTypeHash(const FDirectionAttackKey& Key) 
	{
		return HashCombine(GetTypeHash(Key.Direction), GetTypeHash(static_cast<uint8>(Key.InputType)));
	}

};

struct FGameplayAbilitySpec;
class UGA_AbilityBase;
/**
 * 
 */
UCLASS()
class UGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void ApplyGameplayEffect(TSubclassOf<UGameplayEffect> EffectToApply, int Level = 1);

	void ApplyInitialEffects();
	void GrantInitialAbilities();
	void ApplyFullStat();

	void TryActivateDirectionalAttack(const FVector& Direction, const EAbilityInputID& InputType, bool IsGrounded);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Init")
	TArray<TSubclassOf<UGameplayEffect>> InitialEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Init")
	TSubclassOf<UGameplayEffect> FullStatEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Attacks")
	TArray<TSubclassOf<class UGA_AbilityBase>> LightAttackAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "Attacks")
	TArray<TSubclassOf<class UGA_AbilityBase>> MediumAttackAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "Attacks")
	TArray<TSubclassOf<class UGA_AbilityBase>> HeavyAttackAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "Attacks")
	TArray<TSubclassOf<class UGA_AbilityBase>> SpecialAttackAbilities;

	TArray<FVector> AttackDirections = // Side is neutral attack
	{ 
		FVector(0, 0, 0.5f),	// Up Attack
		FVector(1, 0, 0),		// Neutral/Side Attack
		FVector(0, 0, -1)		// Down Attack
	};

	FVector RoundDirection(FVector inVector);

	void AssignAbilityAttackDirections(const int& Index, EAbilityInputID InputID, FGameplayAbilitySpecHandle SpecHandle);

	UPROPERTY()
	TMap<FDirectionAttackKey, FGameplayAbilitySpecHandle> DirectionToAbilityHandleMap;
};
