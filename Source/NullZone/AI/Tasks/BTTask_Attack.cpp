// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "NullZone/AI/CPP_BaseNPCController.h"
#include "NullZone/AI/CPP_BaseNpc.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = "Attack";
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACPP_BaseNPCController* AI = Cast<ACPP_BaseNPCController>(OwnerComp.GetAIOwner());
	if (!AI) return EBTNodeResult::Failed;
	ACPP_BaseNpc* NPC = Cast<ACPP_BaseNpc>(AI->GetPawn());
	if (!NPC) return EBTNodeResult::Failed;
	NPC->AttackFunc();
	return EBTNodeResult::Succeeded;
}
