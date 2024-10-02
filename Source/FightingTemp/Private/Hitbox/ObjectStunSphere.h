// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectStunSphere.generated.h"

class USphereComponent;

UCLASS()
class AObjectStunSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectStunSphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditDefaultsOnly, Category = "Collider")
	USphereComponent* SphereComponent;

	/*	Stun Player	*/

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	TSet<const AActor*> AlreadyDetectedActors;

	void GetStun(AActor* OtherPlayer);

	/*	Timer	*/
	FTimerHandle StunTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	float StunTime;

	void DestroyStun();

};
