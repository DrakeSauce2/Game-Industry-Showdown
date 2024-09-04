// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GGameplayCueNotify_StaticBase.h"
#include "GameplayAbilities/IGGameplayCueInterface.h"

void UGGameplayCueNotify_StaticBase::PlayHitReaction(AActor* Target)
{
	if (IGGameplayCueInterface* Interafce = Cast<IGGameplayCueInterface>(Target))
	{
		Interafce->PlayHitReaction();
	}
}

void UGGameplayCueNotify_StaticBase::PlayStunAnim(AActor* Target)
{
	if (IGGameplayCueInterface* Interafce = Cast<IGGameplayCueInterface>(Target))
	{
		Interafce->StartStunAnim();
	}
}

void UGGameplayCueNotify_StaticBase::StopStunAnim(AActor* Target)
{
	if (IGGameplayCueInterface* Interafce = Cast<IGGameplayCueInterface>(Target))
	{
		Interafce->StopStunAnim();
	}
}