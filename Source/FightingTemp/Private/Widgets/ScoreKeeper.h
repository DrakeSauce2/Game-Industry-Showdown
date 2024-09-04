// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreKeeper.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class UScoreKeeper : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetPlayerOneScoreMarkActive(int32 MarkIndex);

	UFUNCTION()
	void SetPlayerTwoScoreMarkActive(int32 MarkIndex);

	UFUNCTION()
	void InitScoreMarks();

private:
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget))
	UImage* ScoreMark1;
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget))
	UImage* ScoreMark2;
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget))
	UImage* ScoreMark3;
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget))
	UImage* ScoreMark4;


	UPROPERTY(EditDefaultsOnly, Category = "Score")
	TArray<UImage*> PlayerOneScoreMarks;

	UPROPERTY(EditDefaultsOnly, Category = "Score")
	TArray<UImage*> PlayerTwoScoreMarks;

	UPROPERTY(EditDefaultsOnly, Category = "Score")
	FLinearColor ScoreMarkColor;


};
