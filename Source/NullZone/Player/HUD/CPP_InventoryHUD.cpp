// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_InventoryHUD.h"
#include "TimerManager.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/WidgetSwitcher.h"
#include "NullZone/Player/HUD/CPP_InventoryGrid.h"
#include "NullZone/Player/HUD/CPP_InventorySlot.h"
#include "NullZone/Player/CPP_InventoryComponent.h"



void UCPP_InventoryHUD::LookEvent(float MouseX, float MouseY)
{
	if ((FMath::Abs(MouseX) >= .2f || FMath::Abs(MouseY) >= .2f) && !bIsSlotSelected)
	{
		int32 NewSlot = FMath::FloorToInt32(((FMath::RadiansToDegrees(FMath::Atan2(MouseY, MouseX)) + 180.f) * -1.f) / 60.f) * -1 - 1;
		if (NewSlot == 0) NewSlot = 6;
		if (CurrentSlot == NewSlot) return;
		if (LastSlot != -1 && CurrentSlot == -1) CurrentSlot = LastSlot;
		switch (NewSlot)
		{
		case 1:
			MSlot1Select(true);
			ResetSlot(CurrentSlot);
			break;
		case 2:
			MSlot2Select(true);
			ResetSlot(CurrentSlot);
			break;
		case 3:
			MSlot3Select(true);
			ResetSlot(CurrentSlot);
			break;
		case 4:
			MSlot4Select(true);
			ResetSlot(CurrentSlot);
			break;
		case 5:
			MSlot5Select(true);
			ResetSlot(CurrentSlot);
			break;
		case 6:
			MSlot6Select(true);
			ResetSlot(CurrentSlot);
			break;
		default:
			break;
		}
		CurrentSlot = NewSlot;
//		ResetSlot(LastSlot);
		LastSlot = -1;
		bSlotChosen = true;
	}
}

void UCPP_InventoryHUD::ResetSlot(int32 CurrentSlotTemp)
{
	if (CurrentSlotTemp == -1) return;
	switch (CurrentSlotTemp)
	{
	case 1:
		MSlot1Select(false);
		break;
	case 2:
		MSlot2Select(false);
		break;
	case 3:
		MSlot3Select(false);
		break;
	case 4:
		MSlot4Select(false);
		break;
	case 5:
		MSlot5Select(false);
		break;
	case 6:
		MSlot6Select(false);
		break;
	default:
		break;
	}
	bIsSlotSelected = true;
	GetWorld()->GetTimerManager().ClearTimer(SelectedDelay);
	GetWorld()->GetTimerManager().SetTimer(SelectedDelay, this, &UCPP_InventoryHUD::DelayOnSelected, .2f, false);
}

void UCPP_InventoryHUD::VisibilityHandler(bool bVisibility)
{
	if (!MiscWheel) return;
	if (bVisibility) 
	{
		CurrentSlot = -1;
		InventoryCanvas->SetVisibility(ESlateVisibility::Hidden);
	} 
	MiscWheel->SetVisibility(bVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UCPP_InventoryHUD::MSlot1Select(bool Selected)
{
	if (Selected)
	{
		PlayAnimation(MSlot1Selected, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f, false);
	}
	else
	{
		PlayAnimation(MSlot1Selected, 0.f, 1, EUMGSequencePlayMode::Reverse, 1.f, false);
	}
}

void UCPP_InventoryHUD::MSlot2Select(bool Selected)
{
	if (Selected)
	{
		PlayAnimation(MSlot2Selected, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f, false);
	}
	else
	{
		PlayAnimation(MSlot2Selected, 0.f, 1, EUMGSequencePlayMode::Reverse, 1.f, false);
	}
}

void UCPP_InventoryHUD::MSlot3Select(bool Selected)
{
	if (Selected)
	{
		PlayAnimation(MSlot3Selected, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f, false);
	}
	else
	{
		PlayAnimation(MSlot3Selected, 0.f, 1, EUMGSequencePlayMode::Reverse, 1.f, false);
	}
}

void UCPP_InventoryHUD::MSlot4Select(bool Selected)
{
	if (Selected)
	{
		PlayAnimation(MSlot4Selected, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f, false);
	}
	else
	{
		PlayAnimation(MSlot4Selected, 0.f, 1, EUMGSequencePlayMode::Reverse, 1.f, false);
	}
}

void UCPP_InventoryHUD::MSlot5Select(bool Selected)
{
	if (Selected)
	{
		PlayAnimation(MSlot5Selected, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f, false);
	}
	else
	{
		PlayAnimation(MSlot5Selected, 0.f, 1, EUMGSequencePlayMode::Reverse, 1.f, false);
	}
}

void UCPP_InventoryHUD::MSlot6Select(bool Selected)
{
	if (Selected)
	{
		PlayAnimation(MSlot6Selected, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f, false);
	}
	else
	{
		PlayAnimation(MSlot6Selected, 0.f, 1, EUMGSequencePlayMode::Reverse, 1.f, false);
	}
}

int32 UCPP_InventoryHUD::GetCurrentSlot()
{
	if (!bSlotChosen) return 0;
	if (CurrentSlot == LastSlot && CurrentSlot != -1)
	{
		ResetSlot(LastSlot);
		CurrentSlot = -1;
		LastSlot = CurrentSlot;
	}
	else if (CurrentSlot != -1)
	{
		LastSlot = CurrentSlot;
	}
	bSlotChosen = false;
	return CurrentSlot;
}

void UCPP_InventoryHUD::OnEquipButtonClicked()
{
	if (!CurrentClickedSlot) { UE_LOG(LogTemp, Warning, TEXT("no clicked slot exist")); return; }
	if (EquipText->GetText().ToString().Equals("Equip"))
	{
		ContextBarSwitcher->SetActiveWidgetIndex(1);
	}
	else
	{
		const FItemRuntimeInfo* TempItemData = InventoryComponent->GetItemData(CurrentClickedSlot->GetItemId());
		if (TempItemData == nullptr) { return; }
		InventoryComponent->UnequipItemFromSlot(*TempItemData);
		EquipText->SetText(FText::FromString("Equip"));
		CloseContextMenu();
	}
	
}

void UCPP_InventoryHUD::OnDrop1ButtonClicked()
{
	if (InventoryComponent)
	{
		InventoryComponent->RemoveItem(CurrentClickedSlot->GetItemId(), 1);
		CloseContextMenu();
	}
}

void UCPP_InventoryHUD::OnDropAllButtonClicked()
{
	if (InventoryComponent)
	{
		InventoryComponent->RemoveItem(CurrentClickedSlot->GetItemId(), CurrentClickedSlot->GetItemQuantity());
		CloseContextMenu();
	}
}

void UCPP_InventoryHUD::Slot1MiscEquipFunc()
{
	const FItemRuntimeInfo* TempItemData = InventoryComponent->GetItemData(CurrentClickedSlot->GetItemId());
	if (TempItemData == nullptr) { return; }
	InventoryComponent->EquipItemToSlot(*TempItemData, 1);
	CloseContextMenu();
}
void UCPP_InventoryHUD::Slot2MiscEquipFunc()
{
	const FItemRuntimeInfo* TempItemData = InventoryComponent->GetItemData(CurrentClickedSlot->GetItemId());
	if (TempItemData == nullptr) { return; }
	InventoryComponent->EquipItemToSlot(*TempItemData, 2);
	CloseContextMenu();
}
void UCPP_InventoryHUD::Slot3MiscEquipFunc()
{
	const FItemRuntimeInfo* TempItemData = InventoryComponent->GetItemData(CurrentClickedSlot->GetItemId());
	if (TempItemData == nullptr) { return; }
	InventoryComponent->EquipItemToSlot(*TempItemData, 3);
	CloseContextMenu();
}
void UCPP_InventoryHUD::Slot4MiscEquipFunc()
{
	const FItemRuntimeInfo* TempItemData = InventoryComponent->GetItemData(CurrentClickedSlot->GetItemId());
	if (TempItemData == nullptr) { return; }
	InventoryComponent->EquipItemToSlot(*TempItemData, 4);
	CloseContextMenu();
}
void UCPP_InventoryHUD::Slot5MiscEquipFunc()
{
	const FItemRuntimeInfo* TempItemData = InventoryComponent->GetItemData(CurrentClickedSlot->GetItemId());
	if (TempItemData == nullptr) { return; }
	InventoryComponent->EquipItemToSlot(*TempItemData, 5);
	CloseContextMenu();
}
void UCPP_InventoryHUD::Slot6MiscEquipFunc()
{
	const FItemRuntimeInfo* TempItemData = InventoryComponent->GetItemData(CurrentClickedSlot->GetItemId());
	if (TempItemData == nullptr) { return; }
	InventoryComponent->EquipItemToSlot(*TempItemData, 6);
	CloseContextMenu();
}

void UCPP_InventoryHUD::CompactAmmoDropFunc()
{
	if (!InventoryComponent) return;
	InventoryComponent->RemoveAmmo(EAmmoType::CompactAmmo);
}

void UCPP_InventoryHUD::MachinePistolDropFunc()
{
	if (!InventoryComponent) return;
	InventoryComponent->RemoveAmmo(EAmmoType::MachinePistol);
}

void UCPP_InventoryHUD::ServiceAmmoDropFunc()
{
	if (!InventoryComponent) return;
	InventoryComponent->RemoveAmmo(EAmmoType::ServiceAmmo);
}

void UCPP_InventoryHUD::IntermediateHVDropFunc()
{
	if (!InventoryComponent) return;
	InventoryComponent->RemoveAmmo(EAmmoType::IntermediateHV);
}

void UCPP_InventoryHUD::PrecisionAmmoDropFunc()
{
	if (!InventoryComponent) return;
	InventoryComponent->RemoveAmmo(EAmmoType::PrecisionAmmo);
}

void UCPP_InventoryHUD::LongRangeAmmoDropFunc()
{
	if (!InventoryComponent) return;
	InventoryComponent->RemoveAmmo(EAmmoType::LongRangeAmmo);
}

void UCPP_InventoryHUD::ShotgunShellDropFunc()
{
	if (!InventoryComponent) return;
	InventoryComponent->RemoveAmmo(EAmmoType::ShotgunShell);
}

void UCPP_InventoryHUD::HighCaliberSurplusDropFunc()
{
	if (!InventoryComponent) return;
	InventoryComponent->RemoveAmmo(EAmmoType::HighCaliberSurplus);
}

void UCPP_InventoryHUD::ChangeSlot(int32 SlotNum, const FItemRuntimeInfo& ItemData)
{
	switch (SlotNum)
	{
	case 1:
		MSlot1Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		ISlot1Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		if (ItemData.StaticData.ItemName.Equals("Empty"))
		{
			ISlot1Quantity->SetText(FText::FromString(""));
			WSlot1TotalAmmo->SetText(FText::FromString(""));
			MSlot1Ammo->SetText(FText::FromString(""));
		}
		else
		{
			if (ItemData.StaticData.ItemType == EItemType::Weapon)
			{
				ISlot1Quantity->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(ItemData.WeaponData.MagSize)));
				WSlot1TotalAmmo->SetText(FText::Format(FText::FromString(TEXT("{0}")), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
				MSlot1Ammo->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
			}
			else
			{
				ISlot1Quantity->SetText(ItemData.ItemQuantity == 1 ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
				MSlot1Ammo->SetText(ItemData.StaticData.ItemType == EItemType::None ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
				WSlot1TotalAmmo->SetText(FText::FromString(""));
			}
		}
		break;
	case 2:
		MSlot2Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		ISlot2Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		if (ItemData.StaticData.ItemName.Equals("Empty"))
		{
			ISlot2Quantity->SetText(FText::FromString(""));
			WSlot2TotalAmmo->SetText(FText::FromString(""));
			MSlot2Ammo->SetText(FText::FromString(""));
		}
		else
		{
			if (ItemData.StaticData.ItemType == EItemType::Weapon)
			{
				ISlot2Quantity->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(ItemData.WeaponData.MagSize)));
				WSlot2TotalAmmo->SetText(FText::Format(FText::FromString(TEXT("{0}")), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
				MSlot2Ammo->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
			}
			else
			{
				ISlot2Quantity->SetText(ItemData.ItemQuantity == 1 ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
				MSlot2Ammo->SetText(ItemData.StaticData.ItemType == EItemType::None ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
				WSlot2TotalAmmo->SetText(FText::FromString(""));
			}
		}
		break;
	case 3:
		MSlot3Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		ISlot3Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		if (ItemData.StaticData.ItemName.Equals("Empty"))
		{
			ISlot3Quantity->SetText(FText::FromString(""));
			WSlot3TotalAmmo->SetText(FText::FromString(""));
			MSlot3Ammo->SetText(FText::FromString(""));
		}
		else
		{
			if (ItemData.StaticData.ItemType == EItemType::Weapon)
			{
				ISlot3Quantity->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(ItemData.WeaponData.MagSize)));
				WSlot3TotalAmmo->SetText(FText::Format(FText::FromString(TEXT("{0}")), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
				MSlot3Ammo->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
			}
			else
			{
				ISlot3Quantity->SetText(ItemData.ItemQuantity == 1 ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
				MSlot3Ammo->SetText(ItemData.StaticData.ItemType == EItemType::None ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
				WSlot3TotalAmmo->SetText(FText::FromString(""));
			}
		}
		break;
	case 4:
		MSlot4Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		ISlot4Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		if (ItemData.StaticData.ItemName.Equals("Empty"))
		{
			ISlot4Quantity->SetText(FText::FromString(""));
			WSlot4TotalAmmo->SetText(FText::FromString(""));
			MSlot4Ammo->SetText(FText::FromString(""));
		}
		else
		{
			if (ItemData.StaticData.ItemType == EItemType::Weapon)
			{
				ISlot4Quantity->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(ItemData.WeaponData.MagSize)));
				WSlot4TotalAmmo->SetText(FText::Format(FText::FromString(TEXT("{0}")), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
				MSlot4Ammo->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
			}
			else
			{
				ISlot4Quantity->SetText(ItemData.ItemQuantity == 1 ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
				MSlot4Ammo->SetText(ItemData.StaticData.ItemType == EItemType::None ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
				WSlot4TotalAmmo->SetText(FText::FromString(""));
			}
		}
		break;
	case 5:
		MSlot5Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		ISlot5Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		if (ItemData.StaticData.ItemName.Equals("Empty"))
		{
			ISlot5Quantity->SetText(FText::FromString(""));
			WSlot5TotalAmmo->SetText(FText::FromString(""));
			MSlot5Ammo->SetText(FText::FromString(""));
		}
		else
		{
			if (ItemData.StaticData.ItemType == EItemType::Weapon)
			{
				ISlot5Quantity->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(ItemData.WeaponData.MagSize)));
				WSlot5TotalAmmo->SetText(FText::Format(FText::FromString(TEXT("{0}")), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
				MSlot5Ammo->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
			}
			else
			{
				ISlot5Quantity->SetText(ItemData.ItemQuantity == 1 ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
				MSlot5Ammo->SetText(ItemData.StaticData.ItemType == EItemType::None ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
				WSlot5TotalAmmo->SetText(FText::FromString(""));
			}
		}
		break;
	case 6:
		MSlot6Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		ISlot6Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		if (ItemData.StaticData.ItemName.Equals("Empty"))
		{
			ISlot6Quantity->SetText(FText::FromString(""));
			WSlot6TotalAmmo->SetText(FText::FromString(""));
			MSlot6Ammo->SetText(FText::FromString(""));
		}
		else
		{
			if (ItemData.StaticData.ItemType == EItemType::Weapon)
			{
				ISlot6Quantity->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(ItemData.WeaponData.MagSize)));
				WSlot6TotalAmmo->SetText(FText::Format(FText::FromString(TEXT("{0}")), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
				MSlot6Ammo->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
			}
			else
			{
				ISlot6Quantity->SetText(ItemData.ItemQuantity == 1 ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
				MSlot6Ammo->SetText(ItemData.StaticData.ItemType == EItemType::None ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
				WSlot6TotalAmmo->SetText(FText::FromString(""));
			}
		}
		break;
	default:
		break;
	}
}

void UCPP_InventoryHUD::ChangeAmmo(const EAmmoType& AmmoType, const int32& Quantity)
{
	switch (AmmoType)
	{
	case EAmmoType::CompactAmmo:
		CompactAmmoAmount->SetText(FText::AsNumber(Quantity));
		break;
	case EAmmoType::MachinePistol:
		MachinePistolAmount->SetText(FText::AsNumber(Quantity));
		break;
	case EAmmoType::ServiceAmmo:
		ServiceAmmoAmount->SetText(FText::AsNumber(Quantity));
		break;
	case EAmmoType::IntermediateHV:
		IntermediateHVAmount->SetText(FText::AsNumber(Quantity));
		break;
	case EAmmoType::PrecisionAmmo:
		PrecisionAmmoAmount->SetText(FText::AsNumber(Quantity));
		break;
	case EAmmoType::LongRangeAmmo:
		LongRangeAmount->SetText(FText::AsNumber(Quantity));
		break;
	case EAmmoType::ShotgunShell:
		ShotgunShellAmount->SetText(FText::AsNumber(Quantity));
		break;
	case EAmmoType::HighCaliberSurplus:
		HighCaliberSurplusAmount->SetText(FText::AsNumber(Quantity));
		break;
	default:
		break;
	}
}

void UCPP_InventoryHUD::EquipItem(const FItemRuntimeInfo& ItemInfo)
{
	if (ItemInfo.StaticData.ItemType == EItemType::Weapon)
	{
		EquippedSlotText->SetText(FText::FromString(ItemInfo.StaticData.ItemName));
		EquippedSlotAmmo->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemInfo.WeaponData.Ammos), FText::AsNumber(ItemInfo.WeaponData.MagSize)));
		EquippedSlotTotalAmmo->SetText(FText::Format(FText::FromString(TEXT("{0}")), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemInfo.WeaponData.AmmoType))));
	}
	else if (ItemInfo.StaticData.ItemType == EItemType::Meds)
	{
		EquippedSlotText->SetText(FText::FromString(ItemInfo.StaticData.ItemName));
		EquippedSlotAmmo->SetText(FText::Format(FText::FromString(TEXT("{0}")), FText::AsNumber(ItemInfo.ItemQuantity)));
		EquippedSlotTotalAmmo->SetText(FText::FromString(""));
	}
	else
	{
		EquippedSlotText->SetText(FText::FromString(ItemInfo.StaticData.ItemName));
		EquippedSlotAmmo->SetText(FText::FromString(""));
		EquippedSlotTotalAmmo->SetText(FText::FromString(""));
	}
}

void UCPP_InventoryHUD::InitializeInventory()
{
	if (!InventoryGrid)
	{
		InventoryGrid = CreateWidget<UCPP_InventoryGrid>(GetOwningPlayer(), InventoryGridClass);
		UCanvasPanelSlot* CanvasSlot = InventoryCanvas->AddChildToCanvas(InventoryGrid);
		if (CanvasSlot)
		{
			CanvasSlot->SetAnchors(FAnchors(.5f, .5f));
			CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
			CanvasSlot->SetSize(FVector2D(1050.f, 1080.f));
			CanvasSlot->SetPosition(FVector2D(425.f, 0.f));
		}
	}
	if (ContextMenu)
	{
		ContextMenu->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCPP_InventoryHUD::OnAddedItem(const FItemRuntimeInfo& Items)
{
	if (InventoryGrid)
	{
		InventoryGrid->OnAddedItem(Items);
	}
}

void UCPP_InventoryHUD::OnRemovedItem(FName ItemID, int32 Quanity)
{
	if (InventoryGrid)
	{
		InventoryGrid->OnRemovedItem(ItemID, Quanity);
	}
}

void UCPP_InventoryHUD::DelayOnSelected()
{
	bIsSlotSelected = false;
}

void UCPP_InventoryHUD::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PC = GetOwningPlayer();
	if (!PC) return;

	APawn* Pawn = PC->GetPawn();
	if (!Pawn) return;

	InventoryComponent = Pawn->FindComponentByClass<UCPP_InventoryComponent>();
	if (!InventoryComponent) return;

	InventoryComponent->OnItemAdded.AddDynamic(this, &UCPP_InventoryHUD::OnAddedItem);
	InventoryComponent->OnSlotChanged.AddDynamic(this, &UCPP_InventoryHUD::ChangeSlot);
	InventoryComponent->OnItemRemoved.AddDynamic(this, &UCPP_InventoryHUD::OnRemovedItem);
	InventoryComponent->OnAmmoChanged.AddDynamic(this, &UCPP_InventoryHUD::ChangeAmmo);
	InventoryComponent->OnItemEquipped.AddDynamic(this, &UCPP_InventoryHUD::EquipItem);

	InitializeInventory();

	if (ContextMenu)
	{
		ContextMenuCanvasSlot = Cast<UCanvasPanelSlot>(ContextMenu->Slot);
	}

	EquipButton->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::OnEquipButtonClicked);
	Slot1MiscEquip->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::Slot1MiscEquipFunc);
	Slot2MiscEquip->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::Slot2MiscEquipFunc);
	Slot3MiscEquip->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::Slot3MiscEquipFunc);
	Slot4MiscEquip->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::Slot4MiscEquipFunc);
	Slot5MiscEquip->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::Slot5MiscEquipFunc);
	Slot6MiscEquip->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::Slot6MiscEquipFunc);
	Drop1Button->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::OnDrop1ButtonClicked);
	DropAllButton->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::OnDropAllButtonClicked);
	CompactAmmoDrop->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::CompactAmmoDropFunc);
	MachinePistolDrop->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::MachinePistolDropFunc);
	ServiceAmmoDrop->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::ServiceAmmoDropFunc);
	IntermediateHVDrop->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::IntermediateHVDropFunc);
	PrecisionAmmoDrop->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::PrecisionAmmoDropFunc);
	LongRangeDrop->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::LongRangeAmmoDropFunc);
	ShotgunShellDrop->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::ShotgunShellDropFunc);
	HighCaliberSurplusDrop->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::HighCaliberSurplusDropFunc);

	InventoryCanvas->SetVisibility(ESlateVisibility::Hidden);
	InventoryGrid->OnSlotMenuClicked.AddDynamic(this, &UCPP_InventoryHUD::HandleInventorySlotClicked);
}

void UCPP_InventoryHUD::OnInventoryButtonClicked(bool DoWhat)
{
	if (InventoryCanvas)
	{
		if (DoWhat)
		{
			MiscWheel->SetVisibility(ESlateVisibility::Hidden);
			InventoryCanvas->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			InventoryCanvas->SetVisibility(ESlateVisibility::Hidden);
		}
		
	}
}

void UCPP_InventoryHUD::CloseContextMenu()
{
	ContextMenu->SetVisibility(ESlateVisibility::Hidden);
	ContextBarSwitcher->SetActiveWidgetIndex(0);
	CurrentClickedSlot = nullptr;
	bIsContextMenuVisible = !bIsContextMenuVisible;
}

void UCPP_InventoryHUD::HandleInventorySlotClicked(UCPP_InventorySlot* ClickedSlot)
{
	if (!ContextMenu) { return; }
	
	APlayerController* PC = GetOwningPlayer();
	if (!PC) { return; }

	FVector2D MousePos;
	PC->GetMousePosition(MousePos.X, MousePos.Y);

	constexpr float Offset = -300.f;
	MousePos += FVector2D(Offset, -50.f);

	ContextMenuCanvasSlot->SetPosition(MousePos);

	if (!InventoryComponent->IsItemEquipped(ClickedSlot->GetInstanceID()))
	{
		EquipText->SetText(FText::FromString("Equip"));
	}
	else
	{
		EquipText->SetText(FText::FromString("Unequip"));
	}
	ContextMenu->SetVisibility(bIsContextMenuVisible ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	if (bIsContextMenuVisible) 
	{ 
		ContextBarSwitcher->SetActiveWidgetIndex(0); 
		CurrentClickedSlot = nullptr;
	}
	else
	{
		CurrentClickedSlot = ClickedSlot;
	}
	bIsContextMenuVisible = !bIsContextMenuVisible;
}
