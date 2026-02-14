// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Equippable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEquippable : public UInterface
{
	GENERATED_BODY()
};

class ACPP_PlayerCharacter;

/**
 * 
 */
class NULLZONE_API IEquippable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equippable")
	void StartUseM1(AActor* User, bool bIsPlayer);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equippable")
	void StartUseM2(AActor* User, bool bIsPlayer);

	UFUNCTION(BlueprintNativeEvent)
	void StopUseM1();
	UFUNCTION(BlueprintNativeEvent)
	void StopUseM2();

};
