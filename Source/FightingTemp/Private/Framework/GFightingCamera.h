// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "GFightingCamera.generated.h"

/**
 * 
 */
UCLASS()
class AGFightingCamera : public ACameraActor
{
	GENERATED_BODY()
	
public:
	AGFightingCamera();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	class AGFightingGameMode* GameMode;

	float CalculateDistance();
	FVector CalculatePosition();

	FVector GetPlayerOnePosition();
	FVector GetPlayerTwoPosition();

};
