// Fill out your copyright notice in the Description page of Project Settings.


#include "Hitbox/ObjectHitbox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/GCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AObjectHitbox::AObjectHitbox()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	HitboxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox Component"));
	HitboxComponent->SetCollisionProfileName("Trigger");

	HitboxComponent->SetupAttachment(MeshComponent);
	HitboxComponent->OnComponentBeginOverlap.AddDynamic(this, &AObjectHitbox::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AObjectHitbox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AObjectHitbox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGCharacterBase* OtherPlayer = Cast<AGCharacterBase>(OtherActor);

	if (OtherPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("I found the player!"));
	}
}



