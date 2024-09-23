// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ADialogueBox.generated.h"

class UHitboxComponent;
UCLASS()
class AADialogueBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AADialogueBox();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hitbox")
	UHitboxComponent* HitboxComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle DialogueTimerHandle;
	
	void DestroyDialogue();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timer")
	float DialogueTime;

};
