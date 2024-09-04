// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None			UMETA(DisplayName = "None"),
	Block			UMETA(DisplayName = "Block"),
	Grab			UMETA(DisplayName = "Grab"),
	LightAttack		UMETA(DisplayName = "LightAttack"),
	MediumAttack	UMETA(DisplayName = "MediumAttack"),
	HeavyAttack		UMETA(DisplayName = "HeavyAttack"),
	SpecialAttack	UMETA(DisplayName = "SpecialAttack")
};