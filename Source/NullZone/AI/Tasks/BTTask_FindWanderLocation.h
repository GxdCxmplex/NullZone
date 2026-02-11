// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindWanderLocation.generated.h"

/**
 * 
 */
UCLASS()
class NULLZONE_API UBTTask_FindWanderLocation : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_FindWanderLocation();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
