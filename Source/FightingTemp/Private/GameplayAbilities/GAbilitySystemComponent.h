// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilities/GAbilitySystemTypes.h"
#include "GAbilitySystemComponent.generated.h"

struct FGameplayAbilitySpec;
class UGA_AbilityBase;

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

USTRUCT(BlueprintType)
struct FAttackGroup 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGA_AbilityBase> HighAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGA_AbilityBase> NeutralAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGA_AbilityBase> LowAttack;
};

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
	FAttackGroup LightAttackGroup;
	UPROPERTY(EditDefaultsOnly, Category = "Attacks")
	FAttackGroup MediumAttackGroup;
	UPROPERTY(EditDefaultsOnly, Category = "Attacks")
	FAttackGroup HeavyAttackGroup;
	UPROPERTY(EditDefaultsOnly, Category = "Attacks")
	FAttackGroup SpecialAttackGroup;

	TArray<FVector> AttackDirections = // Side is neutral attack
	{ 
		FVector(0, 0, 0.5f),	// Up Attack
		FVector(1, 0, 0),		// Neutral/Side Attack
		FVector(0, 0, -1)		// Down Attack
	};

	FVector RoundDirection(const FVector& inVector);

	void SetupAttackGroupToAttackMap(FAttackGroup* AttackGroup, const EAbilityInputID& InputType);

	void AssignAbilityAttackDirections(const int& Index, const EAbilityInputID& InputID, const FGameplayAbilitySpecHandle& SpecHandle);

	UPROPERTY()
	TMap<FDirectionAttackKey, FGameplayAbilitySpecHandle> DirectionToAbilityHandleMap;

public:
	FGameplayAbilitySpecHandle AirAttackSpecHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGA_AbilityBase> AirAttack;
};
