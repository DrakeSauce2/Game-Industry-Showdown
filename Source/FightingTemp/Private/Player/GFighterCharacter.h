// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/GCharacterBase.h" // Change this if you change the file name
#include "GFighterCharacter.generated.h"

/*
* 
*/
UCLASS()
class AGFighterCharacter : public AGCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGFighterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	virtual void PawnClientRestart() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
