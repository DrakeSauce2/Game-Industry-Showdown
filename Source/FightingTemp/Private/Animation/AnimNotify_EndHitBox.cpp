// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_EndHitBox.h"

#include "Player/GCharacterBase.h"
#include "Framework/HitboxActor.h"

void UAnimNotify_EndHitBox::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    if (MeshComp && MeshComp->GetOwner())
    {
        AGCharacterBase* Character = Cast<AGCharacterBase>(MeshComp->GetOwner());
        if (Character)
        {
            // Trigger the end of the hitbox
            //AHitboxActor hitbox = Character->GetHitbox();
        }
    }
}
