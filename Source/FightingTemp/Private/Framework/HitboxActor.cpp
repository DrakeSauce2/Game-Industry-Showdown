// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/HitboxActor.h"

#include "Components/BoxComponent.h"

// Sets default values
AHitboxActor::AHitboxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitboxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HitboxComponent"));
	HitboxComponent->SetupAttachment(GetRootComponent());
	HitboxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHitboxActor::EnableHitbox()
{
	HitboxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitboxComponent->SetHiddenInGame(false);
}

void AHitboxActor::DisableHitbox()
{
	HitboxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitboxComponent->SetHiddenInGame(true);
}

void AHitboxActor::SetHitboxTransform(const FVector& Location, const FVector& BoxExtent, const FRotator& Rotation)
{
	SetActorLocation(Location);
	SetActorRotation(Rotation);
	HitboxComponent->SetBoxExtent(BoxExtent);
}

// Called when the game starts or when spawned
void AHitboxActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHitboxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

