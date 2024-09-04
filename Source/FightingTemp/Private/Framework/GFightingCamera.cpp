// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GFightingCamera.h"

#include "Engine/World.h"

#include "Framework/GFightingGameMode.h"

#include "Player/GCharacterBase.h"

AGFightingCamera::AGFightingCamera()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGFightingCamera::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AGFightingGameMode>(GetWorld()->GetAuthGameMode());
}

void AGFightingCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewPosition = FVector(CalculatePosition().X, CalculateDistance(), CalculatePosition().Z + 25.0f);
	FVector SmoothedPosition = FMath::VInterpTo(GetActorLocation(), NewPosition, DeltaTime, 5.0f);
	SetActorLocation(SmoothedPosition);
}

float AGFightingCamera::CalculateDistance()
{
	float Distance = FVector::Dist(GetPlayerOnePosition(), GetPlayerTwoPosition());

	float CameraDistance = Distance * 0.5f + 250.0f; 
	return CameraDistance;
}

FVector AGFightingCamera::CalculatePosition()
{
	return (GetPlayerOnePosition() + GetPlayerTwoPosition()) / 2.0f;
}

FVector AGFightingCamera::GetPlayerOnePosition()
{
	if (!GameMode) return FVector::ZeroVector;

	if(!GameMode->GetPlayerOne()) return FVector::ZeroVector;

	return GameMode->GetPlayerOne()->GetActorLocation();
}

FVector AGFightingCamera::GetPlayerTwoPosition()
{
	if (!GameMode) return FVector::ZeroVector;

	if (!GameMode->GetPlayerTwo()) return FVector::ZeroVector;

	return GameMode->GetPlayerTwo()->GetActorLocation();
}
