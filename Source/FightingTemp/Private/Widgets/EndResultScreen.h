// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndResultScreen.generated.h"

/**
 * 
 */
UCLASS()
class UEndResultScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget))
	class UTextBlock* WinnerText;

	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget))
	class UButton* RematchBtn;

	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget))
	class UButton* MainMenuBtn;

	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget))
	class UButton* CreditsBtn;

private:
	UFUNCTION()
	void Restart();

	UFUNCTION()
	void MainMenu();

	UFUNCTION()
	void Credits();


};
