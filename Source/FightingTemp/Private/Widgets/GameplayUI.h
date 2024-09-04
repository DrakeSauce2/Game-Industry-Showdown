// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/ValueGauge.h"
#include "Widgets/ScoreKeeper.h"
#include "GameplayUI.generated.h"

/**
 * 
 */
UCLASS()
class UGameplayUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetTimerValue(FText Text);
	void SetCountdownValue(FText Text);

	FORCEINLINE UValueGauge* GetPlayerOneHealthBar() { return PlayerOneHealthBar; }
	FORCEINLINE UValueGauge* GetPlayerTwoHealthBar() { return PlayerTwoHealthBar; }

	FORCEINLINE UScoreKeeper* GetScoreKeeper() { return ScoreKeeper; }

private:
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget))
	class UValueGauge* PlayerOneHealthBar;
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget))
	class UValueGauge* PlayerTwoHealthBar;

	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget))
	class UScoreKeeper* ScoreKeeper;

	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget))
	class UTextBlock* TimerText;

	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget))
	class UTextBlock* CountdownText;


};
