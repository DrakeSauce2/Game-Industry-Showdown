// Fill out your copyright notice in the Description page of Project Settings.


#include "Hitbox/ObjectHitbox.h"

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
	GetWorldTimerManager().SetTimer(HitBoxTimerHandle, this, &AObjectHitbox::DestroyHitbox, HitBoxTime, false);

}

void AObjectHitbox::DestroyHitbox()
{
	Destroy();
}

void AObjectHitbox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGCharacterBase* OtherPlayer = Cast<AGCharacterBase>(OtherActor);

	if (OtherPlayer == GetOwner())
	{
		return;
	}

	//if (!AlreadyDetectedActors.Contains(OtherPlayer))
	//{
	//	AlreadyDetectedActors.Add(OtherPlayer);
	//}

	if (OtherPlayer)
	{
		IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherPlayer);
		if (AbilitySystemInterface)
		{
			UE_LOG(LogTemp, Warning, TEXT("They have an ability"));
			GetDamage(OtherPlayer);
		}
	}
}

void AObjectHitbox::GetDamage(AActor* OtherPlayer)
{
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
				UE_LOG(LogTemp, Warning, TEXT("Almost"));
				AbilitySystemComp->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
			}
		}
	}

	//if (OtherPlayer == GetOwner())
	//{
	//	UE_LOG(LogTemp, Error, TEXT("I got the owner"));
	//	return;
	//}

	//if (AlreadyDetectedActors.Contains(OtherPlayer))
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Already Detected"));
	//	return;
	//}

	//UE_LOG(LogTemp, Warning, TEXT("I am damaging!"));

	//FGameplayEventData Data;
	//Data.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(OtherPlayer);
	//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), UGAbilityGenericTags::GetGenericTargetAquiredTag(), Data);
}



