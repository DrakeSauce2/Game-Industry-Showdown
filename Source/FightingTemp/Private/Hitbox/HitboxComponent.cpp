// Fill out your copyright notice in the Description page of Project Settings.


#include "Hitbox/HitboxComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilities/GAbilityGenericTags.h"

void UHitboxComponent::StartDetection()
{
	AlreadyDetectedActors.Empty();
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UHitboxComponent::DoTargetCheck()
{
	TArray<FOverlapResult> OutResult;
	FCollisionShape DetecteionShape;
	const FVector Extend = GetScaledBoxExtent();
	DetecteionShape.SetBox(FVector3f{ (float)Extend.X, (float)Extend.Y, (float)Extend.Z });

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	if (GetWorld()->OverlapMultiByChannel(OutResult,
		GetComponentLocation(),
		GetComponentQuat(),
		ECC_GameTraceChannel1,
		DetecteionShape,
		QueryParams))
	{
		for (const FOverlapResult& result : OutResult)
		{
			AActor* OverlappedActor = result.GetActor();
			if (result.GetComponent() == OverlappedActor->GetRootComponent())
				TargetFound(OverlappedActor);
		}
	}

	DrawDebugBox(GetWorld(), GetComponentLocation(), GetScaledBoxExtent(), FColor::Red, false, 1.f);
}

void UHitboxComponent::EndDetection()
{
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AlreadyDetectedActors.Empty();
}

void UHitboxComponent::TargetFound(AActor* OverlappedActor)
{
	if (OverlappedActor == GetOwner())
	{
		return;
	}

	if (AlreadyDetectedActors.Contains(OverlappedActor))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Found Actor: %s"), *OverlappedActor->GetName());

	AlreadyDetectedActors.Add(OverlappedActor);
	FGameplayEventData Data;
	Data.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(OverlappedActor);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), UGAbilityGenericTags::GetGenericTargetAquiredTag(), Data);
}
