// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.

class ACPP_PickupActor;

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};


class NULLZONE_API IInteractable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickup")
	ACPP_PickupActor* GetPickedUpData() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickup")
	void OnPickedUp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickup")
	void OnBeginHover();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickup")
	void OnEndHover();
};
