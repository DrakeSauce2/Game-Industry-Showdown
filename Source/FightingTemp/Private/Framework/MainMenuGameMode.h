// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainMenuGameMode();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void SpawnMainMenuUI();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UGMainMenu> MainMenuUIClass;
	UGMainMenu* MainMenuUI;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<TSubclassOf<class AGCharacterBase>> CharacterList;

};
