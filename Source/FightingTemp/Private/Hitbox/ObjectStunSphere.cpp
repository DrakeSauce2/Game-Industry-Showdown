// Fill out your copyright notice in the Description page of Project Settings.


#include "Hitbox/ObjectStunSphere.h"

#include "Components/SphereComponent.h"
#include "Player/GCharacterBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilities/GAbilityGenericTags.h"
#include "GameplayAbilities/GAbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "AbilitySystemInterface.h"


// Sets default values
AObjectStunSphere::AObjectStunSphere()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName(TEXT("Trigger"));
	
	RootComponent = SphereComponent;

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AObjectStunSphere::OnOverlapBegin);

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

	if (OtherPlayer != GetOwner())
	{
		UE_LOG(LogTemp, Error, TEXT("They are in the circle"));
		GetStun(OtherPlayer);
	}

}

void AObjectStunSphere::GetStun(AActor* OtherPlayer)
{
	if (AlreadyDetectedActors.Contains(OtherPlayer))
	{
		UE_LOG(LogTemp, Error, TEXT("Player already detected"));
		AlreadyDetectedActors.Remove(OtherPlayer);
		return;
	}

	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherPlayer);
	if (AbilitySystemInterface)
	{
		UAbilitySystemComponent* AbilitySystemComp = AbilitySystemInterface->GetAbilitySystemComponent();
		if (AbilitySystemComp && StunAbility)
		{
			FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComp->MakeOutgoingSpec(StunAbility, 1.0f, AbilitySystemComp->MakeEffectContext());
			if (EffectSpecHandle.IsValid())
			{
				EffectSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("SetByCaller.Duration"), 10.0f); //Not safe - Have a property FGameplayTag
				//UPROPERTY - Only accept SetByCaller tags
				AbilitySystemComp->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
			}
		}
	}

	AlreadyDetectedActors.Add(OtherPlayer);

}

void AObjectStunSphere::DestroyStun()
{
	Destroy();
}

