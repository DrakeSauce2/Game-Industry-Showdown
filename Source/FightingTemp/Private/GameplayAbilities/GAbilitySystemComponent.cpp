// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GAbilitySystemComponent.h"
#include "GameplayAbilities/GA_AbilityBase.h"

void UGAbilitySystemComponent::ApplyGameplayEffect(TSubclassOf<UGameplayEffect> EffectToApply, int Level)
{
	FGameplayEffectSpecHandle specHandle = MakeOutgoingSpec(EffectToApply, Level, MakeEffectContext());
	ApplyGameplayEffectSpecToSelf(*specHandle.Data.Get());
}

void UGAbilitySystemComponent::ApplyInitialEffects()
{
	for (const TSubclassOf<UGameplayEffect>& Effect : InitialEffects)
	{
		ApplyGameplayEffect(Effect, 1);
	}
}

// Maybe Rename to GrantInitialAttackAbilities
void UGAbilitySystemComponent::GrantInitialAbilities()
{
	for (int i = 0; i < LightAttackAbilities.Num(); i++) 
	{
		FGameplayAbilitySpecHandle SpecHandle =
			GiveAbility(FGameplayAbilitySpec{ LightAttackAbilities[i], 1, (int)EAbilityInputID::LightAttack, GetOwner()});

		if (i < AttackDirections.Num()) 
		{
			AssignAbilityAttackDirections(i, EAbilityInputID::LightAttack, SpecHandle);
		}
	}

	for (int i = 0; i < MediumAttackAbilities.Num(); i++)
	{
		FGameplayAbilitySpecHandle SpecHandle =
			GiveAbility(FGameplayAbilitySpec{ MediumAttackAbilities[i], 1, (int)EAbilityInputID::MediumAttack, GetOwner() });

		if (i < AttackDirections.Num())
		{
			AssignAbilityAttackDirections(i, EAbilityInputID::MediumAttack, SpecHandle);
		}
	}

	for (int i = 0; i < MediumAttackAbilities.Num(); i++)
	{
		FGameplayAbilitySpecHandle SpecHandle =
			GiveAbility(FGameplayAbilitySpec{ HeavyAttackAbilities[i], 1, (int)EAbilityInputID::HeavyAttack, GetOwner() });

		if (i < AttackDirections.Num())
		{
			AssignAbilityAttackDirections(i, EAbilityInputID::HeavyAttack, SpecHandle);
		}
	}

	for (int i = 0; i < MediumAttackAbilities.Num(); i++)
	{
		FGameplayAbilitySpecHandle SpecHandle =
			GiveAbility(FGameplayAbilitySpec{ SpecialAttackAbilities[i], 1, (int)EAbilityInputID::SpecialAttack, GetOwner() });

		if (i < AttackDirections.Num())
		{
			AssignAbilityAttackDirections(i, EAbilityInputID::SpecialAttack, SpecHandle);
		}
	}
}

void UGAbilitySystemComponent::ApplyFullStat()
{
	if (FullStatEffect) 
	{
		ApplyGameplayEffect(FullStatEffect);
	}
}

void UGAbilitySystemComponent::TryActivateDirectionalAttack(const FVector& Direction, const EAbilityInputID& InputType, bool IsGrounded)
{
	if (!IsGrounded)
	{
		// Do Air Attack

		return;
	}

	FDirectionAttackKey Key;
	Key.Direction = RoundDirection(Direction);
	Key.InputType = InputType;

	//UE_LOG(LogTemp, Warning, TEXT("FVector: %s, Enum Value: %d"), *RoundDirection(Direction).ToString(), static_cast<int32>(InputType));

	FGameplayAbilitySpecHandle* SpecHandle = DirectionToAbilityHandleMap.Find(Key);
	if (SpecHandle) {
		TryActivateAbility(*SpecHandle, false);
	}
}

FVector UGAbilitySystemComponent::RoundDirection(FVector inVector)
{
	// Abs value because there are no back attacks. we don't need to worry about x direction
	float xDir = FMath::Abs(inVector.X); 
	float zDir = inVector.Z;

	UE_LOG(LogTemp, Warning, TEXT("X: %f, Y: %f"), xDir, zDir);

	if (xDir < 0.5f && zDir < 0.3f && zDir >= 0)
	{
		return FVector(1, 0, 0); // Neutral Attack
	}

	if (xDir > 0.5f && zDir < 0.3f && zDir >= -0.3)
	{
		return FVector(1, 0, 0); // Neutral Attack
	}

	if (xDir > 0.0f && zDir > 0.3f)
	{
		return FVector(0, 0, 0.5f); // Up Attack
	}

	if (xDir >= 0 && zDir < -0.3) 
	{
		return FVector(0, 0, -1); // Down Attack
	}

	return FVector(0, 0, 0);
}

void UGAbilitySystemComponent::AssignAbilityAttackDirections(const int& Index, EAbilityInputID InputID, FGameplayAbilitySpecHandle SpecHandle)
{
	FDirectionAttackKey Key;
	Key.Direction = AttackDirections[Index];
	Key.InputType = InputID;

	DirectionToAbilityHandleMap.Add(Key, SpecHandle);
}
