// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsTargetInRange.h"
#include "NullZone/AI/CPP_BaseNPCController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UBTD_IsTargetInRange::UBTD_IsTargetInRange()
{
	NodeName = "Is Target In Range";
}

bool UBTD_IsTargetInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto* AI = OwnerComp.GetAIOwner();
	if (!AI) return false;

	APawn* AIPawn = AI->GetPawn();
	if (!AIPawn) return false;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return false;

	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!Target) return false;

	const float MinDist = Blackboard->GetValueAsFloat(MinDistanceKey.SelectedKeyName);
	const float MaxDist = Blackboard->GetValueAsFloat(MaxDistanceKey.SelectedKeyName);

	const float Dist = FVector::Dist(AIPawn->GetActorLocation(), Target->GetActorLocation());

	return Dist >= MinDist && Dist <= MaxDist;
}
