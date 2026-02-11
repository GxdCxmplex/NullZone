// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ClearFocus.h"
#include "NullZone/AI/CPP_BaseNPCController.h"

UBTT_ClearFocus::UBTT_ClearFocus()
{
	NodeName = "Clear Focus";
}

EBTNodeResult::Type UBTT_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* AI = OwnerComp.GetAIOwner();

	if (!AI)
	{
		return EBTNodeResult::Failed;
	}
	AI->ClearFocus(EAIFocusPriority::Gameplay);
	return EBTNodeResult::Succeeded;
}

