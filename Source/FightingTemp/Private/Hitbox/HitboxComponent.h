// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "HitboxComponent.generated.h"

/**
 * 
 */
UCLASS()
class UHitboxComponent : public UBoxComponent
{
	GENERATED_BODY()
	
public:
	void StartDetection();
	void DoTargetCheck();
	void EndDetection();

private:
	//Set internally uses hash-table, meaning it has O(1) search time complicty
	TSet<const AActor*> AlreadyDetectedActors;

	void TargetFound(AActor* OverlappedActor);

};
