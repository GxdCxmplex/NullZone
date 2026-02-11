// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindWanderLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "NullZone/AI/CPP_BaseNPCController.h"

#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "NavigationPath.h"
#include "NavigationSystemTypes.h"

UBTTask_FindWanderLocation::UBTTask_FindWanderLocation()
{
	NodeName = TEXT("Find Wander Location");
}

EBTNodeResult::Type UBTTask_FindWanderLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* AI = OwnerComp.GetAIOwner();
	if (!AI) return EBTNodeResult::Failed;
	APawn* CPawn = AI->GetPawn();
	if (!CPawn) return EBTNodeResult::Failed;
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (!NavSys) return EBTNodeResult::Failed;
	FNavLocation NavLoc;
	float WanderRadius = AI->GetBlackboardComponent()->GetValueAsFloat(TEXT("WanderRadius"));
	if (NavSys->GetRandomPointInNavigableRadius(CPawn->GetActorLocation(), WanderRadius, NavLoc))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("PointOfinterest"), NavLoc.Location);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
