// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityAddOns/ADialogueBox.h"
#include "Hitbox/HitboxComponent.h"

// Sets default values
AADialogueBox::AADialogueBox()
{
	DialogueTime = 2.5f;
	HitboxComponent = CreateDefaultSubobject<UHitboxComponent>(TEXT("BoxComponent"));
	RootComponent = HitboxComponent;
}

// Called when the game starts or when spawned
void AADialogueBox::BeginPlay()
{
	Super::BeginPlay();
	
	HitboxComponent->StartDetection();
	HitboxComponent->DoTargetCheck();

	GetWorldTimerManager().SetTimer(DialogueTimerHandle, this, &AADialogueBox::DestroyDialogue, DialogueTime, false);
}

void AADialogueBox::DestroyDialogue()
{
	Destroy();
}

