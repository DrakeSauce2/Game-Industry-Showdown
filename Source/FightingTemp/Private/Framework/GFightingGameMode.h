// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player/GCharacterBase.h"
#include "InputMappingContext.h" 
#include "GFightingGameMode.generated.h"

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

	UPROPERTY()
	AGCharacterBase* PlayerCharacter;

	UPROPERTY()
	int32 Score;

	UPROPERTY()
	FVector StartingPosition;
};

class AGCharacterBase;
/*
* 
*/
UCLASS(minimalapi)
class AGFightingGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGFightingGameMode();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	AGCharacterBase* GetPlayerOne();
	AGCharacterBase* GetPlayerTwo();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	class AGFightingCamera* FightingCamera;

	void SetCameraViewTargets();
	void SetPlayerControllerEnabled(AGCharacterBase* PlayerController, bool state);
	void SetAllPlayersControllerEnabled(bool state);

	
	void UpdatePlayerFlip();

private:
	void DetermineRoundOutcome();
	void HasPlayerWon();

	UFUNCTION()
	void HandlePlayerDead(int32 PlayerID);

	UPROPERTY(EditDefaultsOnly, Category = "Score")
	int32 ScoreToWin = 2;

	int32 CurrentRound = 1;

	void StartRound();
	void EndRound();

private:
	/*****************************************************/
	/*                       Timer                       */
	/*****************************************************/
	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	float RoundStartTime = 60.0f;
	float IntermissionTime = 3.0f;


	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	float CountdownStartTime = 3.0f;

	FTimerHandle RoundTimerHandle;
	float RoundTimeRemaining = -1.0f;

	FTimerHandle CountdownTimerHandle;
	float CountdownTimeRemaining = -1.0f;

	FTimerHandle RoundIntermissionTimerHandle; // Time Between Rounds
	float IntermissionTimeRemaining = -1.0f;

	void UpdateRoundTimer();
	void UpdateCountdownTimer();
	void RoundIntermission();

private:
	/*****************************************************/
	/*                      Spawning                     */
	/*****************************************************/
	void SpawnPlayerControllers();
	AGCharacterBase* SpawnAndPossessCharacter(APlayerController* PlayerController, TSubclassOf<class AGCharacterBase> SelectedCharacterToSpawn, const FVector& SpawnLocation);

	TArray<FPlayerData> PlayerDataArray;

	/*****************************************************/
	/*                      Widgets                      */
	/*****************************************************/
	void SpawnGameplayUI();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UGameplayUI> GameplayUIClass;

	UPROPERTY()
	UGameplayUI* GameplayUI;


};



