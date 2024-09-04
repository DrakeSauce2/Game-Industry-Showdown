// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "GAbilityGenericTags.generated.h"

/**
 * 
 */
UCLASS()
class UGAbilityGenericTags : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static FGameplayTag GetDeadTag();
	static FGameplayTag GetStunnedTag();
	static FGameplayTag GetSpawnVFXCueTag();
	static FGameplayTag GetGenericTargetAquiredTag();
	static FGameplayTag GetBasicAttackAcitvationTag();
};
