// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameplayUI.h"

#include "Components/TextBlock.h"

void UGameplayUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGameplayUI::SetTimerValue(FText Text)
{
	if (!TimerText) return;

	TimerText->SetText(Text);
}

void UGameplayUI::SetCountdownValue(FText Text)
{
	if (!CountdownText) return;

	CountdownText->SetText(Text);
}
