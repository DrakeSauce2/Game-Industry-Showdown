// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_StartHitBox.h"
#include "Hitbox/HitboxComponent.h"

void UAnimNotifyState_StartHitBox::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	const AActor* OwnerActor = MeshComp->GetOwner();
	if (OwnerActor) {
		HitboxComponent = OwnerActor->GetComponentByClass<UHitboxComponent>();
	}

	if (HitboxComponent)
	{
		HitboxComponent->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachSocket);
		HitboxComponent->StartDetection();
	}
}

void UAnimNotifyState_StartHitBox::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (HitboxComponent)
	{
		HitboxComponent->DoTargetCheck();
	}
}

void UAnimNotifyState_StartHitBox::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (HitboxComponent)
	{
		HitboxComponent->EndDetection();
	}
}
