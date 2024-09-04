// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_AbilityBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilities/GAbilityGenericTags.h"

#include "Particles/ParticleSystem.h"

UGA_AbilityBase::UGA_AbilityBase()
{

}

void UGA_AbilityBase::ExecuteSpawnVFXCue(UParticleSystem* VFXToSpawn, float Size, const FVector& Location)
{
	FGameplayCueParameters Params;
	Params.Location = Location;
	Params.RawMagnitude = Size;
	Params.SourceObject = VFXToSpawn;

	GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(UGAbilityGenericTags::GetSpawnVFXCueTag(), Params);
}
