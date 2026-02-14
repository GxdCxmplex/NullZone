// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsTargetInRange.generated.h"

/**
 * 
 */
UCLASS()
class NULLZONE_API UBTD_IsTargetInRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_IsTargetInRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere, Category = "Condition")
	FBlackboardKeySelector MinDistanceKey;

	UPROPERTY(EditAnywhere, Category = "Condition")
	FBlackboardKeySelector MaxDistanceKey;

	UPROPERTY(EditAnywhere, Category = "Condition")
	FBlackboardKeySelector TargetKey;
};
