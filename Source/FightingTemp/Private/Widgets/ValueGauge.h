// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ValueGauge.generated.h"

/**
 * 
 */
UCLASS()
class UValueGauge : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void SetValue(float NewVal, float NewMaxValue);

private:
	UPROPERTY(EditAnywhere, Category = "Visual")
		FLinearColor FillColor;

	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget))
		class UProgressBar* ProgressBar;
};
