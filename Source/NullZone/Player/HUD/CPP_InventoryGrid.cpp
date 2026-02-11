// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_InventoryGrid.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"



void UCPP_InventoryGrid::BuildGrid(const TArray<FItemRuntimeInfo>& Items)
{
	GridPanel->ClearChildren();
	CreatedSlots.Empty();

	for (const FItemRuntimeInfo& Item : Items)
	{
		AddItemToGrid(Item);
	}
}

void UCPP_InventoryGrid::OnAddedItem(const FItemRuntimeInfo& ItemInfo)
{
	for (UCPP_InventorySlot* CurrontSlot : CreatedSlots)
	{
		if (CurrontSlot->GetItemId() == ItemInfo.StaticData.ItemId && CurrontSlot->GetIsStackable())
		{
			CurrontSlot->SetQuanity(ItemInfo.ItemQuantity);
			return;
		}
	}
	AddItemToGrid(ItemInfo);
}



void UCPP_InventoryGrid::OnRemovedItem(FName ItemId, int32 ItemQuanity)
{
	if (!GridPanel)
		return;

	for (int32 i = 0; i < CreatedSlots.Num(); i++)
	{
			if (CreatedSlots[i]->GetItemId() == ItemId)
			{
				if (CreatedSlots[i]->GetItemQuantity() > ItemQuanity)
				{
					CreatedSlots[i]->SetQuanity(CreatedSlots[i]->GetItemQuantity() - ItemQuanity);
					
				}
				else if (CreatedSlots[i]->GetItemQuantity() == ItemQuanity)
				{
					GridPanel->RemoveChild(CreatedSlots[i]);
					CreatedSlots.RemoveAt(i);
					RefreshGrid();
					GridPanel->InvalidateLayoutAndVolatility();
				}
				break;
			}
			
	}
}

void UCPP_InventoryGrid::AddItemToGrid(const FItemRuntimeInfo& ItemInfo)
{
	if (!GridPanel || !SlotWidgetClass) return;
	UCPP_InventorySlot* NewSlot = CreateWidget<UCPP_InventorySlot>(GetOwningPlayer(), SlotWidgetClass);
	
	if (!NewSlot) return;	
	NewSlot->SetItemData(ItemInfo);

	int32 Index = CreatedSlots.Num();
	int32 Row = Index / Columns;
	int32 Column = Index % Columns;

	UUniformGridSlot* GridSlot = GridPanel->AddChildToUniformGrid(NewSlot, Row, Column);
	GridSlot->SetHorizontalAlignment(HAlign_Center);
	GridSlot->SetVerticalAlignment(VAlign_Center);
	CreatedSlots.Add(NewSlot);
	NewSlot->OnSlotMenuClicked.AddDynamic(this, &UCPP_InventoryGrid::HandleMenuButtonClick);
}

void UCPP_InventoryGrid::RefreshGrid()
{
	for (int32 i = 0; i < CreatedSlots.Num(); i++)
	{
		if (UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(CreatedSlots[i]->Slot))
		{
			UE_LOG(LogTemp, Warning, TEXT("Refresh"))
			GridSlot->SetRow((i) / Columns);
			GridSlot->SetColumn((i) % Columns);
		}
	}
}

void UCPP_InventoryGrid::HandleMenuButtonClick(UCPP_InventorySlot* ClickedSlot)
{
	OnSlotMenuClicked.Broadcast(ClickedSlot);
}
