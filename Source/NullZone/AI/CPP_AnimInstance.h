// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CPP_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NULLZONE_API UCPP_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void AnimNotify_SwingStart();

	UFUNCTION()
	void AnimNotify_SwingEnd();
	
};
