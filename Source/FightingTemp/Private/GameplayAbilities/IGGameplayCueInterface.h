// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IGGameplayCueInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGGameplayCueInterface : public UInterface
{
	GENERATED_BODY()
};


/**
 * 
 */
class IGGameplayCueInterface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void PlayHitReaction() = 0; //this a pure virtual function. also made the class abstract.
	virtual void StartStunAnim() = 0;
	virtual void StopStunAnim() = 0;
};
