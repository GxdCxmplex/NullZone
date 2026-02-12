// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HUD/CPP_PlayerHUD.h"
#include "HUD/CPP_InventoryHUD.h"
#include "CPP_PlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStaminaChangedDelegate, float, Old, float, Current, float, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChangedDelegate, float, Old, float, Current, float, Max);
/**
 * 
 */
UCLASS()
class NULLZONE_API ACPP_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ACPP_PlayerController();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaChangedDelegate OnStaminaChanged;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedDelegate OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateStaminaFunc(float Old, float Current, float Max);
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateHealthFunc(float Old, float Current, float Max);

	void OnInventoryOpen(bool dowhat);

	void ChangeInteractionText(const FString& NewText);

	void WeaponWheelVisibilityHandler(bool bVisibility);

	void WeaponWheelLookEvent(float MouseX, float MouseY);

	void UpdateMainProgressBarPercent(float Percent, FString Name);

	int32 GetCurrentWeaponSlot();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCPP_PlayerHUD> PlayerHUDClass;

	UPROPERTY()
	UCPP_PlayerHUD* HUD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCPP_InventoryHUD> InventoryHUDClass;

	UPROPERTY()
	UCPP_InventoryHUD* InvHUD;


public:
	UCPP_PlayerHUD* GetHUDWidget() const { return HUD; }
	UCPP_InventoryHUD* GetInvHUDWidget() const { return InvHUD; }
};
