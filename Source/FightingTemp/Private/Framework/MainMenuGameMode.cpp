// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/MainMenuGameMode.h"

#include "Widgets/GMainMenu.h"

AMainMenuGameMode::AMainMenuGameMode()
{

}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	SpawnMainMenuUI();
}

void AMainMenuGameMode::SpawnMainMenuUI()
{
	if (!MainMenuUIClass) return;

	MainMenuUI = CreateWidget<UGMainMenu>(GetWorld(), MainMenuUIClass);
	if (MainMenuUI)
	{
		MainMenuUI->AddToViewport();
	}
}