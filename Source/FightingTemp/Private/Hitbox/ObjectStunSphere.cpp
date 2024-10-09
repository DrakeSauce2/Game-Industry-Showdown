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

	if (OtherPlayer == GetOwner())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Got Owner, do nothing."));
	}

	if (OtherPlayer)
	{
		//IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherPlayer);
		//if (AbilitySystemInterface)
		//{
		//	//UE_LOG(LogTemp, Warning, TEXT("Player has an ability"));
		//	GetStun(OtherPlayer);
		//}
	}

}

void AObjectStunSphere::GetStun(AActor* OtherPlayer)
{
	//if (OtherPlayer == GetOwner())
	//{
	//	//UE_LOG(LogTemp, Error, TEXT("I got the owner, do nothing"));
	//}

	//if (AlreadyDetectedActors.Contains(OtherPlayer))
	//{
	//	//UE_LOG(LogTemp, Error, TEXT("Already Detected"));
	//	AlreadyDetectedActors.Remove(OtherPlayer);
	//	return;
	//}

	////Stun the Player
	//IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherPlayer);
	//if (AbilitySystemInterface)
	//{
	//	UAbilitySystemComponent* AbilitySystemComp = AbilitySystemInterface->GetAbilitySystemComponent();
	//	if (AbilitySystemComp && StunAbility)
	//	{
	//		if (OtherPlayer != GetOwner())
	//		{
	//			FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComp->MakeOutgoingSpec(StunAbility, 1.0f, AbilitySystemComp->MakeEffectContext());
	//			if (EffectSpecHandle.IsValid())
	//			{
	//				UE_LOG(LogTemp, Warning, TEXT("Stun Away"));
	//				AbilitySystemComp->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	//			}
	//		}
	//	}
	//}

	//AlreadyDetectedActors.Add(OtherPlayer);
}

void AObjectStunSphere::DestroyStun()
{
	Destroy();
}

