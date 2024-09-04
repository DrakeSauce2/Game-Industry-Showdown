// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GCharacterBase.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

#include "Components/CapsuleComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Framework/HitboxActor.h"

#include "GameplayAbilities/GAbilitySystemComponent.h"
#include "GameplayAbilities/GAbilitySystemTypes.h"
#include "GameplayAbilities/GAttributeSet.h"
#include "GameplayAbilities/GAbilityGenericTags.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Hitbox/HitboxComponent.h"
#include "Hitbox/PushboxComponent.h"

#include "Widgets/ValueGauge.h"

AGCharacterBase::AGCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UGAttributeSet>(TEXT("AttributeSet"));

	HitboxComponent = CreateDefaultSubobject<UHitboxComponent>("Hitbox Component");
	HitboxComponent->SetupAttachment(GetMesh());

	PushboxComponent = CreateDefaultSubobject<UPushboxComponent>("Pushbox Component");
	PushboxComponent->SetupAttachment(GetMesh());

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGAttributeSet::GetHealthAttribute()).AddUObject(this, &AGCharacterBase::HealthUpdated);
	AbilitySystemComponent->RegisterGameplayTagEvent(UGAbilityGenericTags::GetDeadTag()).AddUObject(this, &AGCharacterBase::DeathTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(UGAbilityGenericTags::GetStunnedTag()).AddUObject(this, &AGCharacterBase::StunTagChanged);
}

UAbilitySystemComponent* AGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AGCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	StartingPosition = GetActorLocation();
}

// Called every frame
void AGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	*	Force constrains the players position, the options in the editor didn't work
	*	so I just hard coded it in. Probably try to sort that out later.
	*/ 
	if (GetActorLocation().Y != StartingPosition.Y)
	{
		FVector NewLocation = GetActorLocation();
		NewLocation.Y = StartingPosition.Y;

		SetActorLocation(NewLocation);
	}
}

// Called to bind functionality to input
void AGCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComp)
	{
		enhancedInputComp->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AGCharacterBase::HandleDirectionalInput);
		enhancedInputComp->BindAction(MoveInputAction, ETriggerEvent::Completed, this, &AGCharacterBase::DirectionalInputEnd);
		enhancedInputComp->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &AGCharacterBase::Jump);

		for (const TPair<EAbilityInputID, UInputAction*>& InputPair : AttackInputMap)
		{
			enhancedInputComp->BindAction(InputPair.Value, ETriggerEvent::Started, this, &AGCharacterBase::HandleAbilityInput, InputPair.Key);
		}

		//enhancedInputComp->BindAction(BlockInputAction, ETriggerEvent::Triggered, this, &AGCharacterBase::Block);
	}
}

void AGCharacterBase::HandleDirectionalInput(const FInputActionValue& InputValue)
{
	PlayerInput = InputValue.Get<FVector2D>();
	PlayerInput.Normalize();

	// TODO: implement crouching

	FVector Direction = FVector(PlayerInput.X, 0, 0);

	AddMovementInput(Direction);

	if (PlayerInput.Y > 0 && PlayerInput.X < 0.6f && PlayerInput.X > -0.6f)
	{
		Jump();
	}
}

void AGCharacterBase::DirectionalInputEnd(const FInputActionValue& InputValue)
{
	PlayerInput = FVector2D::ZeroVector;
}

void AGCharacterBase::HealthUpdated(const FOnAttributeChangeData& ChangeData)
{
	if (HealthBar)
	{
		HealthBar->SetValue(ChangeData.NewValue, AttributeSet->GetMaxHealth());
	}

	if (ChangeData.NewValue <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("DEAD"));

		if (GetMesh()->GetAnimInstance())
		{
			GetMesh()->GetAnimInstance()->StopAllMontages(1.0f);
		}

		PlayMontage(DeathMontage);
		AbilitySystemComponent->ApplyGameplayEffect(DeathEffect);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		int32 PlayerID = GetPlayerLocalID();
		OnPlayerDeath.Broadcast(PlayerID);
	}
}

void AGCharacterBase::FlipCharacter(bool bIsFacingRight)
{
	float direction = bIsFacingRight ? 1.0f : -1.0f;

	if (GetMesh()->GetComponentScale().Y == direction) return;

	FVector NewScale = GetMesh()->GetComponentScale();
	NewScale.Y = direction;

	GetMesh()->SetWorldScale3D(NewScale);
}

#pragma region Init Functions

void AGCharacterBase::InitAttributes()
{
	AbilitySystemComponent->ApplyInitialEffects();
}

void AGCharacterBase::InitAbilities()
{
	AbilitySystemComponent->GrantInitialAbilities();
}

void AGCharacterBase::SetHealthBar(UValueGauge* HealthBarToSet)
{
	if (HealthBarToSet)
	{
		HealthBar = HealthBarToSet;
	}
}

#pragma endregion 

#pragma region Gameplay Cue Functions

void AGCharacterBase::StartStunAnim()
{
	PlayMontage(StunMontage);
}

void AGCharacterBase::StopStunAnim()
{
	GetMesh()->GetAnimInstance()->Montage_Stop(0.2, StunMontage);
}

void AGCharacterBase::PlayHitReaction()
{
	PlayMontage(HitReactionMontage);
}

#pragma endregion 

#pragma region Helper Functions

float AGCharacterBase::GetHealth()
{
	if (!AttributeSet) return 0.0f;

	return AttributeSet->GetHealth();
}

void AGCharacterBase::ApplyFullStat()
{
	if (AbilitySystemComponent)
		AbilitySystemComponent->ApplyFullStat();
}

void AGCharacterBase::SetInputEnabled(bool state)
{
	if (state == true) 
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	else 
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
}

FVector2D AGCharacterBase::GetPlayerInput() const
{
	return PlayerInput;
}

void AGCharacterBase::PlayMontage(UAnimMontage* MontageToPlay)
{
	if (MontageToPlay)
	{
		if (GetMesh()->GetAnimInstance())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
		}
	}
}

void AGCharacterBase::StunTagChanged(const FGameplayTag TagChanged, int32 NewStackCount)
{
	if (NewStackCount != 0)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		TArray<FGameplayAbilitySpec> AllAbilities = GetAbilitySystemComponent()->GetActivatableAbilities();
		for (FGameplayAbilitySpec& Spec : AllAbilities)
		{
			GetAbilitySystemComponent()->CallServerEndAbility(Spec.Handle, Spec.ActivationInfo, FPredictionKey());
		}
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void AGCharacterBase::DeathTagChanged(const FGameplayTag TagChanged, int32 NewStackCount)
{
	if (NewStackCount == 0)
	{
		StopAnimMontage(DeathMontage);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

int32 AGCharacterBase::GetPlayerLocalID() const
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			return LocalPlayer->GetControllerId();
		}
	}

	return -1;  // No Valid Local Player ID Found
}

#pragma endregion 

#pragma region Action Functions

void AGCharacterBase::HandleAbilityInput(EAbilityInputID InputId)
{
	UE_LOG(LogTemp, Warning, TEXT("Handling Ability Input"));

	AbilitySystemComponent->TryActivateDirectionalAttack
	(
		FVector(PlayerInput.X, 0, PlayerInput.Y), 
		InputId, 
		!GetCharacterMovement()->IsFalling()
	);
}

#pragma endregion
