// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NullZone/AI/SetMovementSpeed.h"
#include "BTT_SetMovementSpeed.generated.h"

/**
 * 
 */
UCLASS()
class NULLZONE_API UBTT_SetMovementSpeed : public UBTTaskNode, public ISetMovementSpeed
{
	GENERATED_BODY()
	
public:
	UBTT_SetMovementSpeed();

	UPROPERTY(EditAnywhere, Category = "Movement")
	EMovementSpeed MovementSpeed;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
