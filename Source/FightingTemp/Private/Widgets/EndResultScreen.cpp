// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/EndResultScreen.h"

#include "Components/Button.h"

#include "Engine/World.h"

#include "Kismet/GameplayStatics.h"

void UEndResultScreen::NativeConstruct()
{
	Super::NativeConstruct();

	RematchBtn->OnClicked.AddDynamic(this, &UEndResultScreen::Restart);
	MainMenuBtn->OnClicked.AddDynamic(this, &UEndResultScreen::MainMenu);
	CreditsBtn->OnClicked.AddDynamic(this, &UEndResultScreen::Credits);
}

void UEndResultScreen::Restart()
{
	UE_LOG(LogTemp, Warning, TEXT("Restarting!"));
}

void UEndResultScreen::MainMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("Main Menuing or something!"));
	UGameplayStatics::OpenLevel(GetWorld(), FName("Main_Menu"));

	/*
	*	Handles Garabage Collection, The game will crash trying to re - enter The fight scene again from main menu! 
	*	Idk why Open Level Handles other garbage collection but not all garbage collection.
	* 
	*/ 
	APlayerController* PlayerTwoController = UGameplayStatics::GetPlayerController(GetWorld(), 1);
	if (PlayerTwoController != nullptr)
	{
		UGameplayStatics::RemovePlayer(PlayerTwoController, true);
	}
}

void UEndResultScreen::Credits()
{
	UE_LOG(LogTemp, Warning, TEXT("Credits!"));

}
