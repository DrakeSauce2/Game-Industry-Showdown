// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "PushboxComponent.generated.h"

/**
 * 
 */
UCLASS()
class UPushboxComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UPushboxComponent();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnPushboxBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Pushbox")
	float PushForceStrength;
};
