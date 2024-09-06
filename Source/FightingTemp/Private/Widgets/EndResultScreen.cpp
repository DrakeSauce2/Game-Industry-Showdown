// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/EndResultScreen.h"

#include "Components/Button.h"

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

}

void UEndResultScreen::Credits()
{
	UE_LOG(LogTemp, Warning, TEXT("Credits!"));

}
