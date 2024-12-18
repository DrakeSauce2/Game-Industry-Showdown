// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectHitbox.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UGameplayEffect;
class UGAbilitySystemComponent;

UCLASS()
class AObjectHitbox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectHitbox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, Category = "Hitbox")
	UBoxComponent* HitboxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Hitbox")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	float PushBackAmount;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void GetDamage(AActor* OtherPlayer);

	/*	Ability System	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<UGameplayEffect> DamageAbility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UGAbilitySystemComponent* AbilitySystemComponent;

	TSet<const AActor*> AlreadyDetectedActors;

	/*	Timer	*/
	FTimerHandle HitBoxTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	float HitBoxTime;

	void DestroyHitbox();

};
