// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_InventorySlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UCPP_InventorySlot::SetItemData(const FItemRuntimeInfo& ItemData)
{
	if (ItemCount)
	{
		if (ItemData.ItemQuantity > 1)
		{
			ItemCount->SetText(FText::AsNumber(ItemData.ItemQuantity));
		}
		else
		{
			ItemCount->SetText(FText::FromString(""));
		}
	}

	if (ItemName)
	{
		ItemName->SetText(FText::FromString(ItemData.StaticData.ItemName));
	}
	ItemID = ItemData.StaticData.ItemId;
	bIsStackable = ItemData.StaticData.bIsStackable;
	Type = ItemData.StaticData.ItemType;
	Quantity = ItemData.ItemQuantity;
	InstanceID = ItemData.InstanceId;
}

void UCPP_InventorySlot::SetQuanity(int32 Quanity)
{
	if (ItemCount)
	{
		ItemCount->SetText(Quanity == 1 ? FText::FromString("") : FText::AsNumber(Quanity));
		Quantity = Quanity;
	}
}


void UCPP_InventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (MenuButton)
	{
		MenuButton->OnClicked.AddDynamic(this, &UCPP_InventorySlot::OnMenuButtonClick);
	}
}

void UCPP_InventorySlot::OnMenuButtonClick()
{
	OnSlotMenuClicked.Broadcast(this);
}
