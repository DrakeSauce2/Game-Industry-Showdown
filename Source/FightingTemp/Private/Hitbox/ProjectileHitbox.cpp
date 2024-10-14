// Fill out your copyright notice in the Description page of Project Settings.


#include "Hitbox/ProjectileHitbox.h"

// Sets default values
AProjectileHitbox::AProjectileHitbox()
{
}

// Called when the game starts or when spawned
void AProjectileHitbox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileHitbox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AProjectileHitbox::GetDamage(AActor* OtherPlayer)
{
}

void AProjectileHitbox::DestroyHitbox()
{
}

