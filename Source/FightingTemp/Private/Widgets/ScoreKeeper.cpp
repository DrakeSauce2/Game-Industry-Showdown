// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ScoreKeeper.h"

#include "Components/Image.h"

void UScoreKeeper::NativeConstruct()
{
	Super::NativeConstruct();
}

void UScoreKeeper::SetPlayerOneScoreMarkActive(int32 MarkIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("I AM USELESS! index: %d"), MarkIndex);

	if (PlayerOneScoreMarks.Num() == 0) return;

	UE_LOG(LogTemp, Warning, TEXT("I AM NOT DOING SOMETHING!"));

	if (!PlayerOneScoreMarks[MarkIndex]) return;

	UE_LOG(LogTemp, Warning, TEXT("I AM DOING SOMETHING!"));

	PlayerOneScoreMarks[MarkIndex]->SetColorAndOpacity(ScoreMarkColor);
}

void UScoreKeeper::SetPlayerTwoScoreMarkActive(int32 MarkIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("I AM USELESS!"));

	if (PlayerTwoScoreMarks.Num() == 0) return;

	UE_LOG(LogTemp, Warning, TEXT("I AM NOT DOING SOMETHING!"));
	
	if (!PlayerTwoScoreMarks[MarkIndex]) return;

	UE_LOG(LogTemp, Warning, TEXT("I AM DOING SOMETHING!"));

	PlayerTwoScoreMarks[MarkIndex]->SetColorAndOpacity(ScoreMarkColor);
}

void UScoreKeeper::InitScoreMarks()
{
	UE_LOG(LogTemp, Warning, TEXT("INITIALIZING!"));

	PlayerOneScoreMarks.Add(ScoreMark1);
	PlayerOneScoreMarks.Add(ScoreMark2);

	PlayerTwoScoreMarks.Add(ScoreMark3);
	PlayerTwoScoreMarks.Add(ScoreMark4);
}
