// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GAbilitySystemComponent.h"
#include "GameplayAbilities/GA_AbilityBase.h"

#pragma region Apply Effect Functions

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

void UGAbilitySystemComponent::ApplyFullStat()
{
	if (FullStatEffect)
	{
		ApplyGameplayEffect(FullStatEffect);
	}
}

#pragma endregion 

#pragma region Attack Ability Functions

/*
*		Goes through the FAttackGroup struct and gives the GameplayAbilitySystem it the ability
*		return the ability spec handle. It then passes it into the AssignAbilityAttackDirections()
*		function where it adds it to the DirectionToAbilityHandleMap. This map is used to Query for
*		directional attacks on attack input using the EAbilityInputID and FVector as a key.
*/

void UGAbilitySystemComponent::GrantInitialAbilities()
{
	SetupAttackGroupToAttackMap(&LightAttackGroup, EAbilityInputID::LightAttack);
	SetupAttackGroupToAttackMap(&MediumAttackGroup, EAbilityInputID::MediumAttack);
	SetupAttackGroupToAttackMap(&HeavyAttackGroup, EAbilityInputID::HeavyAttack);
	SetupAttackGroupToAttackMap(&SpecialAttackGroup, EAbilityInputID::SpecialAttack);
}

void UGAbilitySystemComponent::SetupAttackGroupToAttackMap(FAttackGroup* AttackGroup, const EAbilityInputID& InputType)
{
	FGameplayAbilitySpecHandle HighAttackSpecHandle =
		GiveAbility(FGameplayAbilitySpec{ AttackGroup->HighAttack, 1, (int)InputType, GetOwner() });
	FGameplayAbilitySpecHandle NeutralAttackSpecHandle =
		GiveAbility(FGameplayAbilitySpec{ AttackGroup->NeutralAttack, 1, (int)InputType, GetOwner() });
	FGameplayAbilitySpecHandle LowAttackSpecHandle =
		GiveAbility(FGameplayAbilitySpec{ AttackGroup->LowAttack, 1, (int)InputType, GetOwner() });

	AssignAbilityAttackDirections(0, InputType, HighAttackSpecHandle);
	AssignAbilityAttackDirections(1, InputType, NeutralAttackSpecHandle);
	AssignAbilityAttackDirections(2, InputType, LowAttackSpecHandle);
}

void UGAbilitySystemComponent::AssignAbilityAttackDirections(const int& Index, const EAbilityInputID& InputType, const FGameplayAbilitySpecHandle& SpecHandle)
{
	FDirectionAttackKey Key;
	Key.Direction = AttackDirections[Index];
	Key.InputType = InputType;

	DirectionToAbilityHandleMap.Add(Key, SpecHandle);

	AirLightSpecHandle = GiveAbility(FGameplayAbilitySpec{ AirLight, 1, 1, GetOwner() });
	AirMediumSpecHandle = GiveAbility(FGameplayAbilitySpec{ AirMedium, 1, 1, GetOwner() });
	AirHeavySpecHandle = GiveAbility(FGameplayAbilitySpec{ AirHeavy, 1, 1, GetOwner() });
	AirSpecialSpecHandle = GiveAbility(FGameplayAbilitySpec{ AirSpecial, 1, 1, GetOwner() });
}

#pragma endregion 

void UGAbilitySystemComponent::TryActivateDirectionalAttack(const FVector& Direction, const EAbilityInputID& InputType, bool IsGrounded)
{
	if (!IsGrounded)
	{
		// Do Air Attack
		// FGameplayAbilitySpecHandle SpecHandle = AirAttackSpecHandle;
		//bool AbilityActive = TryActivateAbility(SpecHandle, false);
		//UE_LOG(LogTemp, Error, TEXT("Has something happened: %s"), AbilityActive ? TEXT("True") : TEXT("False"));
		
		FGameplayAbilitySpecHandle SpecHandle;
		bool AbilityActive;

		switch ((int)InputType)
		{
		case 3U:
			UE_LOG(LogTemp, Error, TEXT("Light Attack"));
			SpecHandle = AirLightSpecHandle;
			AbilityActive = TryActivateAbility(SpecHandle, false);
			break;
		case 4U:
			UE_LOG(LogTemp, Error, TEXT("Medium Attack"));
			SpecHandle = AirMediumSpecHandle;
			AbilityActive = TryActivateAbility(SpecHandle, false);
			break;
		case 5U:
			UE_LOG(LogTemp, Error, TEXT("Heavy Attack"));
			SpecHandle = AirHeavySpecHandle;
			AbilityActive = TryActivateAbility(SpecHandle, false);
			break;
		case 6U:
			UE_LOG(LogTemp, Error, TEXT("Special Attack"));
			SpecHandle = AirSpecialSpecHandle;
			AbilityActive = TryActivateAbility(SpecHandle, false);
			break;
		default:
			break;
		}

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

#pragma region Helper Functions 

FVector UGAbilitySystemComponent::RoundDirection(const FVector& inVector)
{
	// Abs value because there are no back attacks. we don't need to worry about x direction
	float xDir = FMath::Abs(inVector.X);
	float zDir = inVector.Z;

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

#pragma endregion



