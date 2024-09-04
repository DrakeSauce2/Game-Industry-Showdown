// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GAbilityGenericTags.h"

FGameplayTag UGAbilityGenericTags::GetDeadTag()
{
	return FGameplayTag::RequestGameplayTag("stat.dead");
}

FGameplayTag UGAbilityGenericTags::GetStunnedTag()
{
	return FGameplayTag::RequestGameplayTag("stat.stun");
}

FGameplayTag UGAbilityGenericTags::GetSpawnVFXCueTag()
{
	return FGameplayTag::RequestGameplayTag("GameplayCue.spawnVFX");
}

FGameplayTag UGAbilityGenericTags::GetGenericTargetAquiredTag()
{
	return FGameplayTag::RequestGameplayTag("targeting.aquired");
}

FGameplayTag UGAbilityGenericTags::GetBasicAttackAcitvationTag()
{
	return FGameplayTag::RequestGameplayTag("ability.attack.activate");
}
