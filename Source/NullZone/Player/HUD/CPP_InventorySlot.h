// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NullZone/Data/ItemInfo.h"
#include "CPP_InventorySlot.generated.h"

class UTextBlock;
class UImage;
class UButton;
class UCPP_InventorySlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotMenuClickedDelegate, UCPP_InventorySlot*, Slot);

/**
 * 
 */
UCLASS()
class NULLZONE_API UCPP_InventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetItemData(const FItemRuntimeInfo& ItemData);
	void SetQuanity(int32 Quanity);

	FName GetItemId() { return ItemID; }
	bool GetIsStackable() { return bIsStackable; }
	EItemType GetItemType() { return Type; }
	int32 GetItemQuantity() { return Quantity; }
	FGuid GetInstanceID() { return InstanceID; }
	
	UPROPERTY(BlueprintAssignable)
	FOnSlotMenuClickedDelegate OnSlotMenuClicked;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnMenuButtonClick();

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCount;

	UPROPERTY(meta = (BindWidget))
	UButton* MenuButton;

	UPROPERTY()
	FName ItemID;

	UPROPERTY()
	FGuid InstanceID;

	UPROPERTY()
	bool bIsStackable;

	UPROPERTY()
	EItemType Type;

	UPROPERTY()
	int32 Quantity;
};
