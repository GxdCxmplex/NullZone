// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NullZone/Data/ItemInfo.h"
#include "CPP_InventorySlot.h"
#include "CPP_InventoryGrid.generated.h"


class UCPP_InventorySlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotMenuClicked, UCPP_InventorySlot*, Slot);
/**
 * 
 */
UCLASS()
class NULLZONE_API UCPP_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void BuildGrid(const TArray<FItemRuntimeInfo>& Items);
	void OnAddedItem(const FItemRuntimeInfo& ItemInfo);
	void OnRemovedItem(FName ItemId, int32 ItemQuanity);
	void AddItemToGrid(const FItemRuntimeInfo& ItemInfo);
	void RefreshGrid();
	UFUNCTION()
	void HandleMenuButtonClick(UCPP_InventorySlot* ClickedSlot);

	UPROPERTY(BlueprintAssignable)
	FOnSlotMenuClicked OnSlotMenuClicked;

protected:
	UPROPERTY(meta=(BindWidget))
	class UUniformGridPanel* GridPanel;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UCPP_InventorySlot> SlotWidgetClass;

	//UFUNCTION()
	//void OnMenuButtonClick();

private:
	TArray<UCPP_InventorySlot*> CreatedSlots;
	int32 Columns = 5;
};
