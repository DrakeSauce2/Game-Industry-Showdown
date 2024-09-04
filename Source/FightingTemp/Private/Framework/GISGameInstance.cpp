// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GISGameInstance.h"

void UGISGameInstance::SetMatchRoundTimeLimit(float TimeToSet)
{
	TimeToSet = FMath::Clamp(TimeToSet, 60, 180);

	MatchRoundTimeLimit = TimeToSet;
}

void UGISGameInstance::SetScoreToWin(int ScoreToSet)
{
	ScoreToSet = FMath::Clamp(ScoreToSet, 1, 3);

	ScoreToWin = ScoreToSet;
}

float UGISGameInstance::GetMatchRoundTimeLimit()
{
	return MatchRoundTimeLimit;
}

int UGISGameInstance::GetScoreToWin()
{
	return ScoreToWin;
}
