// Fill out your copyright notice in the Description page of Project Settings.


#include "Hitbox/ObjectStunSphere.h"
#include "Components/SphereComponent.h"
#include "Player/GCharacterBase.h"

// Sets default values
AObjectStunSphere::AObjectStunSphere()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName(TEXT("Trigger"));
	
	RootComponent = SphereComponent;

	//Remove these once the effects are set - MR
	SphereComponent->SetHiddenInGame(false);
	SphereComponent->SetVisibility(true);

}

// Called when the game starts or when spawned
void AObjectStunSphere::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(StunTimerHandle, this, &AObjectStunSphere::DestroyStun, StunTime, false);
}

void AObjectStunSphere::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGCharacterBase* OtherPlayer = Cast<AGCharacterBase>(OtherActor);

	if (OtherPlayer == GetOwner())
	{
		return;
	}

	if (OtherPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player in the Stun Sphere"));
		GetStun(OtherPlayer);
	}

}

void AObjectStunSphere::GetStun(AActor* OtherPlayer)
{
	if (OtherPlayer == GetOwner())
	{
		UE_LOG(LogTemp, Error, TEXT("I got the owner"));
		return;
	}

	if (AlreadyDetectedActors.Contains(OtherPlayer))
	{
		UE_LOG(LogTemp, Error, TEXT("Already Detected"));
		AlreadyDetectedActors.Remove(OtherPlayer);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Stunnning"));
	//Stun the Player

	AlreadyDetectedActors.Add(OtherPlayer);
}

void AObjectStunSphere::DestroyStun()
{
	Destroy();
}

