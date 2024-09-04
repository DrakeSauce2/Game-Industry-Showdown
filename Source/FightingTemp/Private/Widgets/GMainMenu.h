// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class UGMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

private:
	TSubclassOf<class UButton> CharacterButtonClass;

};
