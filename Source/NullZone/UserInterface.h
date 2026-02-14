// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NullZone/Data/ItemInfo.h"
#include "UserInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUserInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NULLZONE_API IUserInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    virtual float GetStamina() const = 0;
    virtual float GetStaminaMax() const = 0;
    virtual void SetStamina(float NewValue, float MaxValue) = 0;

    virtual float GetHealth() const = 0;
    virtual float GetHealthMax() const = 0;
    virtual void SetHealth(float NewValue, float MaxValue) = 0;

    virtual bool CanAttack() const = 0;
    virtual void SetAttackBool(bool bValue) = 0;

    virtual void UpdateProgressBar(float Progress, FString ItemName) = 0;

    virtual void MedApplied(FItemRuntimeInfo Data) = 0;

    virtual class UCPP_InventoryComponent* GetInventoryComponent() const = 0;
    virtual class ACPP_PlayerController* GetPlayerController() const = 0;
};
