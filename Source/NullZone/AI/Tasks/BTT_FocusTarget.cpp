// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FocusTarget.h"
#include "NullZone/AI/CPP_BaseNPCController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTT_FocusTarget::UBTT_FocusTarget()
{
	NodeName = "Focus Target";
}

EBTNodeResult::Type UBTT_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* AI = OwnerComp.GetAIOwner();
	if (!AI) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (!Target) return EBTNodeResult::Failed;

	AI->SetFocus(Target, EAIFocusPriority::Gameplay);

	return EBTNodeResult::Succeeded;
}
