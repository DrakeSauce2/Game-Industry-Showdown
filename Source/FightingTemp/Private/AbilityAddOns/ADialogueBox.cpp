// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityAddOns/ADialogueBox.h"

// Sets default values
AADialogueBox::AADialogueBox()
{
	DialogueTime = 2.5f;
}

// Called when the game starts or when spawned
void AADialogueBox::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(DialogueTimerHandle, this, &AADialogueBox::DestroyDialogue, DialogueTime, false);
}

void AADialogueBox::DestroyDialogue()
{
	Destroy();
}

