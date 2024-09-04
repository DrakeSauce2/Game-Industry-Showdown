// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GISGameInstance.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class UGISGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Character Selection")
	TSubclassOf<class AGCharacterBase> SelectedCharacterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMapping;

	void SetMatchRoundTimeLimit(float TimeToSet);
	void SetScoreToWin(int ScoreToSet);

	float GetMatchRoundTimeLimit();
	int GetScoreToWin();

private:
	float MatchRoundTimeLimit = 90;
	int ScoreToWin = 2;

};
