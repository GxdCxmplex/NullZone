// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetMovementSpeed.h"
#include "NullZone/AI/CPP_BaseNPCController.h"

UBTT_SetMovementSpeed::UBTT_SetMovementSpeed()
{
	NodeName = "Set Movement Speed";
}

EBTNodeResult::Type UBTT_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* AI = OwnerComp.GetAIOwner();
	if (!AI) return EBTNodeResult::Failed;

	APawn* Pwn = AI->GetPawn();
	if (!Pwn) return EBTNodeResult::Failed;

	if (Pwn->Implements<USetMovementSpeed>())
	{
		ISetMovementSpeed::Execute_SetMovementSpeed(Pwn, MovementSpeed);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
