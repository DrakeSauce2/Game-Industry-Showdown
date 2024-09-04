// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GGameplayCueNotify_StaticBase.generated.h"

/**
 * 
 */
UCLASS()
class UGGameplayCueNotify_StaticBase : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay Cue")
	static void PlayHitReaction(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Cue")
	static void PlayStunAnim(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Cue")
	static void StopStunAnim(AActor* Target);
};
