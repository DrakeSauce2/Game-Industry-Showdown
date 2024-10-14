// Fill out your copyright notice in the Description page of Project Settings.


#include "Hitbox/ProjectileHitbox.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/GCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilities/GAbilityGenericTags.h"
#include "GameplayAbilities/GAbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileHitbox::AProjectileHitbox()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	HitboxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox Component"));
	HitboxComponent->SetCollisionProfileName("Trigger");

	HitboxComponent->SetupAttachment(MeshComponent);
	HitboxComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileHitbox::OnOverlapBegin);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Comp"));
	ProjectileMovement->UpdatedComponent = MeshComponent;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->bConstrainToPlane = true;
	ProjectileMovement->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);
}

// Called when the game starts or when spawned
void AProjectileHitbox::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(HitBoxTimerHandle, this, &AProjectileHitbox::DestroyHitbox, HitBoxTime, false);
}

void AProjectileHitbox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGCharacterBase* OtherPlayer = Cast<AGCharacterBase>(OtherActor);

	if (OtherPlayer == GetOwner())
	{
		return;
	}

	if (OtherPlayer)
	{
		IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherPlayer);
		if (AbilitySystemInterface)
		{
			UE_LOG(LogTemp, Warning, TEXT("They have an ability"));
			GetDamage(OtherPlayer);
		}

		//Call to player to be pushed, wait to merge

	}
}

void AProjectileHitbox::GetDamage(AActor* OtherPlayer)
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

	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherPlayer);
	if (AbilitySystemInterface)
	{
		UAbilitySystemComponent* AbilitySystemComp = AbilitySystemInterface->GetAbilitySystemComponent();
		if (AbilitySystemComp && DamageAbility)
		{
			//UE_LOG(LogTemp, Warning, TEXT("We might be close"));

			FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComp->MakeOutgoingSpec(DamageAbility, 1.0f, AbilitySystemComp->MakeEffectContext());
			if (EffectSpecHandle.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("(V)ictory Damage!"));
				AbilitySystemComp->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
			}
		}
	}

	AlreadyDetectedActors.Add(OtherPlayer);
}

void AProjectileHitbox::DestroyHitbox()
{
	Destroy();
}

