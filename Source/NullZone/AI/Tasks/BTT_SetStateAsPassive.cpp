// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetStateAsPassive.h"
#include "NullZone/AI/CPP_BaseNPCController.h"

UBTT_SetStateAsPassive::UBTT_SetStateAsPassive()
{
	NodeName = "Set State Passive";
}

EBTNodeResult::Type UBTT_SetStateAsPassive::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACPP_BaseNPCController* AI = Cast<ACPP_BaseNPCController>(OwnerComp.GetAIOwner());
	if (!AI) return EBTNodeResult::Failed;

	AI->SetGeneralState(ENpcGeneralState::Passive);
	return EBTNodeResult::Succeeded;
}
