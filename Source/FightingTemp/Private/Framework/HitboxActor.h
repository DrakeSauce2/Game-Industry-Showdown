// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitboxActor.generated.h"

class UBoxComponent;
/*
* 
*/
UCLASS()
class AHitboxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitboxActor();

	void EnableHitbox();
	void DisableHitbox();
	void SetHitboxTransform(const FVector& Location, const FVector& BoxExtent, const FRotator& Rotation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Hitbox")
	UBoxComponent* HitboxComponent;

};
