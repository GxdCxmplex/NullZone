// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AnimInstance.h"
#include "CPP_BaseNpc.h"
#include "NullZone/Equippable.h"

void UCPP_AnimInstance::AnimNotify_SwingStart()
{
    ACPP_BaseNpc* Npc = Cast<ACPP_BaseNpc>(TryGetPawnOwner());
    if (Npc)
    {
        Npc->LightAttackBegin();
    }
}

void UCPP_AnimInstance::AnimNotify_SwingEnd()
{
    ACPP_BaseNpc* Npc = Cast<ACPP_BaseNpc>(TryGetPawnOwner());
    if (Npc)
    {
        Npc->OnAttackEnd();
    }
}
