// Copyright Epic Games, Inc. All Rights Reserved.

#include "Framework/GFightingGameMode.h"

#include "GameplayAbilities/GAbilitySystemComponent.h"

#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"

#include "Framework/GFightingCamera.h"

#include "GameFramework/PlayerController.h"

#include "Kismet/GameplayStatics.h"

#include "Framework/GISGameInstance.h"

#include "Player/GFighterCharacter.h"
#include "Player/GCharacterBase.h"

#include "TimerManager.h"

#include "UObject/ConstructorHelpers.h"

#include "Widgets/GameplayUI.h"
#include "Widgets/ScoreKeeper.h"

/*
*	As far as I've researched about local co-op in UE5 it currently does not work
*	with one gamepad and a keyboard. The keyboard and gamepad will just control Player 0,
*	to fix this you must use two gamepads.
*	
*	If you are testing this on keyboard and cannot control both players then that is
*	the issue
*/

AGFightingGameMode::AGFightingGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGFightingGameMode::BeginPlay()
{
	Super::BeginPlay();

	FRotator SpawnRotation = FRotator(0.0f, -90.0f, 0.0f);
	FActorSpawnParameters SpawnParams;
	FightingCamera = GetWorld()->SpawnActor<AGFightingCamera>(AGFightingCamera::StaticClass(), FVector::ZeroVector, SpawnRotation, SpawnParams);

	SpawnGameplayUI();
	GameplayUI->GetScoreKeeper()->InitScoreMarks();

	SpawnPlayerControllers();

	GetPlayerOne()->SetHealthBar(GameplayUI->GetPlayerOneHealthBar());
	GetPlayerOne()->InitAttributes();
	GetPlayerOne()->InitAbilities();

	GetPlayerTwo()->SetHealthBar(GameplayUI->GetPlayerTwoHealthBar());
	GetPlayerTwo()->InitAttributes();
	GetPlayerTwo()->InitAbilities();


	/*
	*	This is to add a delay between the players spawning and the setting the camera view targets
	*	I don't why but Unreal doesn't use this camera without the delay
	*/ 
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AGFightingGameMode::SetCameraViewTargets, 0.1f, false);

	SetAllPlayersControllerEnabled(false);

	CountdownTimeRemaining = CountdownStartTime;

	if (GameplayUI)
	{
		FText Text = FText::Format(FText::FromString("Round: {0}"), FText::AsNumber(CurrentRound));
		GameplayUI->SetCountdownValue(Text);
	}

	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AGFightingGameMode::UpdateCountdownTimer, 1.0f, true);
}

void AGFightingGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePlayerFlip();
}

#pragma region Helper Functions

void AGFightingGameMode::SetCameraViewTargets()
{
	APlayerController* PlayerOneController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerOneController && FightingCamera)
	{
		PlayerOneController->SetViewTarget(FightingCamera);
	}

	APlayerController* PlayerTwoController = UGameplayStatics::GetPlayerController(GetWorld(), 1);
	if (PlayerTwoController && FightingCamera)
	{
		PlayerTwoController->SetViewTarget(FightingCamera);
	}
}

void AGFightingGameMode::SetPlayerControllerEnabled(AGCharacterBase* Player, bool state)
{
	if (!Player) return;

	Player->SetInputEnabled(state);
}

void AGFightingGameMode::SetAllPlayersControllerEnabled(bool state)
{
	for (const FPlayerData& PlayerData : PlayerDataArray)
	{
		SetPlayerControllerEnabled(PlayerData.PlayerCharacter, state);
	}
}

AGCharacterBase* AGFightingGameMode::GetPlayerOne()
{
	return PlayerDataArray[0].PlayerCharacter;
}

AGCharacterBase* AGFightingGameMode::GetPlayerTwo()
{
	return PlayerDataArray[1].PlayerCharacter;
}

#pragma endregion

#pragma region Gameplay Functions

void AGFightingGameMode::StartRound()
{
	if (GetWorldTimerManager().IsTimerActive(RoundTimerHandle)) return;

	RoundTimeRemaining = RoundStartTime;

	if (GameplayUI)
	{
		FText Text = FText::Format(FText::FromString("Round: {0}"), FText::AsNumber(CurrentRound));
		GameplayUI->SetCountdownValue(Text);
	}

	GetWorldTimerManager().SetTimer(RoundTimerHandle, this, &AGFightingGameMode::UpdateRoundTimer, 1.0f, true);

	// Do some other logic here like making sure the player is fully healed and start fight countdown
}

void AGFightingGameMode::EndRound()
{
	if (GetWorldTimerManager().IsTimerActive(RoundTimerHandle)) 
	{
		GetWorldTimerManager().ClearTimer(RoundTimerHandle); 
	}

	SetAllPlayersControllerEnabled(false);

	HasPlayerWon();

	// Maybe wait for a bit for animations or fade-in/fade-out then reset for next round or match end
}

/*
*	Determines Round Outcome When Timer Runs Out
*/
void AGFightingGameMode::DetermineRoundOutcome()
{
	if (FMath::IsNearlyEqual(PlayerDataArray[0].PlayerCharacter->GetHealth(), PlayerDataArray[1].PlayerCharacter->GetHealth(), 0.01f))
	{
		if (GameplayUI) 
		{
			FText Text = FText::FromString("Draw");
			GameplayUI->SetCountdownValue(Text);
		}
		
		return;
	}

	if (PlayerDataArray[0].PlayerCharacter->GetHealth() > PlayerDataArray[1].PlayerCharacter->GetHealth())
	{
		PlayerDataArray[0].Score++;

		if (GameplayUI)
			GameplayUI->GetScoreKeeper()->SetPlayerOneScoreMarkActive(PlayerDataArray[0].Score - 1);
	}
	else
	{
		PlayerDataArray[1].Score++;

		if (GameplayUI)
			GameplayUI->GetScoreKeeper()->SetPlayerTwoScoreMarkActive(PlayerDataArray[1].Score - 1);
	}
}

void AGFightingGameMode::HasPlayerWon()
{	
	for (int i = 0; i < PlayerDataArray.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player %d Score: %d"), i, PlayerDataArray[i].Score)

		if (PlayerDataArray[i].Score == ScoreToWin)
		{
			if (GameplayUI)
			{
				// Alternatively we could use the character name here
				FText Text = FText::Format(FText::FromString("Player {0} Wins!"), FText::AsNumber(i));
				GameplayUI->SetCountdownValue(Text);
			}

			// TODO: Do win stuff. Go Back To Main Menu

			return; // Found a winner
		}
	}

	/* v	Did not find a winner, continue match	v */

	CurrentRound++;

	IntermissionTimeRemaining = IntermissionTime;
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AGFightingGameMode::RoundIntermission, 1.0f, true);
}

void AGFightingGameMode::HandlePlayerDead(int32 PlayerID)
{
	UE_LOG(LogTemp, Warning, TEXT("Handling Dead Person: %d"), PlayerID);

	// this is jank but, its so ensure it doesn't double kill
	if (!GetWorldTimerManager().IsTimerActive(RoundTimerHandle)) return; 

	if (PlayerID == 0)
	{
		PlayerDataArray[1].Score++;

		if (GameplayUI)
			GameplayUI->GetScoreKeeper()->SetPlayerTwoScoreMarkActive(PlayerDataArray[1].Score - 1);
	}
	else 
	{
		PlayerDataArray[0].Score++;

		if (GameplayUI)
			GameplayUI->GetScoreKeeper()->SetPlayerOneScoreMarkActive(PlayerDataArray[0].Score - 1);
	}
		
	EndRound();
}

void AGFightingGameMode::UpdatePlayerFlip()
{
	if (!GetPlayerOne() || !GetPlayerTwo()) return;

	float PlayerOneXPosition = GetPlayerOne()->GetActorLocation().X;
	float PlayerTwoXPosition = GetPlayerTwo()->GetActorLocation().X;

	float MidpointX = (PlayerOneXPosition + PlayerTwoXPosition) / 2.0f;

	// If true is facing right, else is facing left
	GetPlayerOne()->FlipCharacter(PlayerOneXPosition < MidpointX);
	GetPlayerTwo()->FlipCharacter(PlayerTwoXPosition < MidpointX);
}

#pragma endregion

#pragma region Timer Update Functions

void AGFightingGameMode::UpdateRoundTimer()
{
	if (!GameplayUI) return;

	RoundTimeRemaining -= 1.0f;

	FText Text = FText::AsNumber(FMath::FloorToInt(RoundTimeRemaining));
	GameplayUI->SetTimerValue(Text);

	if (RoundTimeRemaining <= 0)
	{
		DetermineRoundOutcome();

		EndRound();
	}
}


void AGFightingGameMode::UpdateCountdownTimer()
{
	if (!GameplayUI) return;

	CountdownTimeRemaining -= 1.0f;

	/*
	FText Text = FText::AsNumber(FMath::FloorToInt(CountdownTimeRemaining));
	GameplayUI->SetCountdownValue(Text);
	*/

	if (GameplayUI)
	{
		FText Text = FText::Format(FText::FromString("Round: {0}"), FText::AsNumber(CurrentRound));
		GameplayUI->SetCountdownValue(Text);
	}

	if (CountdownTimeRemaining <= 0.0f)
	{
		StartRound();

		SetAllPlayersControllerEnabled(true);

		GameplayUI->SetCountdownValue(FText::FromString("FIGHT!"));
	}

	if (CountdownTimeRemaining <= -0.5f)
	{
		GameplayUI->SetCountdownValue(FText::FromString(""));

		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
	}
}

void AGFightingGameMode::RoundIntermission()
{
	IntermissionTimeRemaining -= 1.0f;

	if (IntermissionTimeRemaining <= 0) // Start Next Round
	{
		GetWorldTimerManager().ClearTimer(RoundIntermissionTimerHandle);

		for (const FPlayerData& PlayerData : PlayerDataArray)
		{
			PlayerData.PlayerCharacter->ApplyFullStat();

			PlayerData.PlayerCharacter->SetActorLocation(PlayerData.StartingPosition);
		}

		// Player Back To Start Positions

		CountdownTimeRemaining = CountdownStartTime;
		GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AGFightingGameMode::UpdateCountdownTimer, 1.0f, true);
	}
}

#pragma endregion

#pragma region Player Spawning Functions

void AGFightingGameMode::SpawnPlayerControllers()
{
	UGISGameInstance* GameInstance = Cast<UGISGameInstance>(GetGameInstance());
	if (!GameInstance || !GameInstance->SelectedCharacterClass) return;


	
	FPlayerData PlayerOneData;
	PlayerOneData.Score = 0;
	APlayerController* PlayerOneController = UGameplayStatics::GetPlayerController(GetWorld(), 0); // We don't need to create a local player for player 1 as its created for us automatically
	if (PlayerOneController)
	{
		FVector SpawnLocation = FVector(810.0f, 10.0f, 200.0f); // Adjust as needed
		PlayerOneData.StartingPosition = SpawnLocation;

		// TODO: Implement character selection choice properly
		PlayerOneData.PlayerCharacter = SpawnAndPossessCharacter(PlayerOneController, GameInstance->SelectedCharacterClass, SpawnLocation);
		PlayerDataArray.Add(PlayerOneData);

		PlayerOneData.PlayerCharacter->OnPlayerDeath.AddDynamic(this, &AGFightingGameMode::HandlePlayerDead);
	}

	FPlayerData PlayerTwoData;
	PlayerTwoData.Score = 0;
	APlayerController* PlayerTwoController = UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
	if (PlayerTwoController)
	{
		FVector SpawnLocation = FVector(1290.0f, 10.0f, 200.0f); // Adjust as needed
		PlayerTwoData.StartingPosition = SpawnLocation;

		// TODO: Implement character selection choice properly
		PlayerTwoData.PlayerCharacter = SpawnAndPossessCharacter(PlayerTwoController, GameInstance->SelectedCharacterClass, SpawnLocation);
		PlayerDataArray.Add(PlayerTwoData);

		PlayerTwoData.PlayerCharacter->OnPlayerDeath.AddDynamic(this, &AGFightingGameMode::HandlePlayerDead);
	}
}

AGCharacterBase* AGFightingGameMode::SpawnAndPossessCharacter(APlayerController* PlayerController, TSubclassOf<class AGCharacterBase> SelectedCharacterToSpawn, const FVector& SpawnLocation)
{
	UGISGameInstance* GameInstance = Cast<UGISGameInstance>(GetGameInstance());
	if (!GameInstance) return nullptr; 

	if (!PlayerController) return nullptr;

	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player);
	if (!LocalPlayer) return nullptr;

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!InputSubsystem) return nullptr;

	// Add Input Mapping before possess character. Results vary if applied after possession.
	InputSubsystem->AddMappingContext(GameInstance->InputMapping, 0);

	PlayerController->SetInputMode(FInputModeGameOnly());

	FRotator SpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerController;
	
	// Create Selected Character
	AGCharacterBase* InstancedCharacter = GetWorld()->SpawnActor<AGCharacterBase>(SelectedCharacterToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
	if (!InstancedCharacter) return nullptr;

	// Owning Controller Possesses Selected Character
	PlayerController->Possess(InstancedCharacter);
	UE_LOG(LogTemp, Warning, TEXT("Player spawned and possessed successfully!"));

	return InstancedCharacter;
}

#pragma endregion
 
#pragma region UI Functions

void AGFightingGameMode::SpawnGameplayUI()
{
	if (!GameplayUIClass) return;

	GameplayUI = CreateWidget<UGameplayUI>(GetWorld(), GameplayUIClass);
	if (GameplayUI)
	{
		GameplayUI->AddToViewport();
	}
}

#pragma endregion


