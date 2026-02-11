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
		int32 NewSlot = FMath::RoundToInt32(((FMath::RadiansToDegrees(FMath::Atan2(MouseY, MouseX)) + 180.f) * -1.f) / 90.f) * -1;
		if (NewSlot == 0) return;
		if (CurrentSlot == NewSlot) return;
		if (LastSlot != -1 && CurrentSlot == -1) CurrentSlot = LastSlot;
		switch (NewSlot)
		{
		case 1:
			Slot1Select(true);
			ResetSlot(CurrentSlot);
			break;
		case 2:
			Slot2Select(true);
			ResetSlot(CurrentSlot);
			break;
		case 3:
			Slot3Select(true);
			ResetSlot(CurrentSlot);
			break;
		case 4:
			Slot4Select(true);
			ResetSlot(CurrentSlot);
			break;
		default:
			break;
		}
		CurrentSlot = NewSlot;
		ResetMSlot(MLastSlot);
		MLastSlot = -1;
		bSlotChosen = true;
	}
}

void UCPP_InventoryHUD::MLookEvent(float MouseX, float MouseY)
{
	if ((FMath::Abs(MouseX) >= .2f || FMath::Abs(MouseY) >= .2f) && !bIsMSlotSelected)
	{
		int32 NewSlot = FMath::FloorToInt32(((FMath::RadiansToDegrees(FMath::Atan2(MouseY, MouseX)) + 180.f) * -1.f) / 60.f) * -1 - 1;
		if (NewSlot == 0) NewSlot = 6;
		if (MCurrentSlot == NewSlot) return;
		if (MLastSlot != -1 && MCurrentSlot == -1) MCurrentSlot = MLastSlot;
		switch (NewSlot)
		{
		case 1:
			MSlot1Select(true);
			ResetMSlot(MCurrentSlot);
			break;
		case 2:
			MSlot2Select(true);
			ResetMSlot(MCurrentSlot);
			break;
		case 3:
			MSlot3Select(true);
			ResetMSlot(MCurrentSlot);
			break;
		case 4:
			MSlot4Select(true);
			ResetMSlot(MCurrentSlot);
			break;
		case 5:
			MSlot5Select(true);
			ResetMSlot(MCurrentSlot);
			break;
		case 6:
			MSlot6Select(true);
			ResetMSlot(MCurrentSlot);
			break;
		default:
			break;
		}
		MCurrentSlot = NewSlot;
		ResetSlot(LastSlot);
		LastSlot = -1;
		bMSlotChosen = true;
	}
}

void UCPP_InventoryHUD::ResetSlot(int32 CurrentSlotTemp)
{
	if (CurrentSlotTemp == -1) return;
	switch (CurrentSlotTemp)
	{
	case 1:
		Slot1Select(false);
		break;
	case 2:
		Slot2Select(false);
		break;
	case 3:
		Slot3Select(false);
		break;
	case 4:
		Slot4Select(false);
		break;
	default:
		break;
	}
	bIsSlotSelected = true;
	GetWorld()->GetTimerManager().ClearTimer(SelectedDelay);
	GetWorld()->GetTimerManager().SetTimer(SelectedDelay, this, &UCPP_InventoryHUD::DelayOnSelected, .2f, false);
}

void UCPP_InventoryHUD::ResetMSlot(int32 CurrentSlotTemp)
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
	bIsMSlotSelected = true;
	GetWorld()->GetTimerManager().ClearTimer(MSelectedDelay);
	GetWorld()->GetTimerManager().SetTimer(MSelectedDelay, this, &UCPP_InventoryHUD::MDelayOnSelected, .2f, false);
}


void UCPP_InventoryHUD::VisibilityHandler(bool bVisibility)
{
	if (!WeaponSelector) return;
	if (bVisibility)
	{
		CurrentSlot = -1;
		MiscWheel->SetVisibility(ESlateVisibility::Hidden);
		InventoryCanvas->SetVisibility(ESlateVisibility::Hidden);
	}
	WeaponSelector->SetVisibility(bVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UCPP_InventoryHUD::MVisibilityHandler(bool bVisibility)
{
	if (!MiscWheel) return;
	if (bVisibility) 
	{
		MCurrentSlot = -1;
		WeaponSelector->SetVisibility(ESlateVisibility::Hidden);
		InventoryCanvas->SetVisibility(ESlateVisibility::Hidden);
	} 
	MiscWheel->SetVisibility(bVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UCPP_InventoryHUD::Slot1Select(bool Selected)
{
	if (Selected)
	{
		PlayAnimation(Slot1Selected, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f, false);
	}
	else
	{
		PlayAnimation(Slot1Selected, 0.f, 1, EUMGSequencePlayMode::Reverse, 1.f, false);
	}
}

void UCPP_InventoryHUD::Slot2Select(bool Selected)
{
	if (Selected)
	{
		PlayAnimation(Slot2Selected, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f, false);
	}
	else
	{
		PlayAnimation(Slot2Selected, 0.f, 1, EUMGSequencePlayMode::Reverse, 1.f, false);
	}
}

void UCPP_InventoryHUD::Slot3Select(bool Selected)
{
	if (Selected)
	{
		PlayAnimation(Slot3Selected, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f, false);
	}
	else
	{
		PlayAnimation(Slot3Selected, 0.f, 1, EUMGSequencePlayMode::Reverse, 1.f, false);
	}
}

void UCPP_InventoryHUD::Slot4Select(bool Selected)
{
	if (Selected)
	{
		PlayAnimation(Slot4Selected, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f, false);
	}
	else
	{
		PlayAnimation(Slot4Selected, 0.f, 1, EUMGSequencePlayMode::Reverse, 1.f, false);
	}
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

int32 UCPP_InventoryHUD::GetCurrentMSlot()
{
	if (!bMSlotChosen) return 0;
	if (MCurrentSlot == MLastSlot && MCurrentSlot != -1)
	{
		ResetSlot(MLastSlot);
		MCurrentSlot = -1;
		MLastSlot = MCurrentSlot;
	}
	else if (MCurrentSlot != -1)
	{
		MLastSlot = MCurrentSlot;
	}
	bMSlotChosen = false;
	return MCurrentSlot;
}

void UCPP_InventoryHUD::OnEquipButtonClicked()
{
	if (!CurrentClickedSlot) { UE_LOG(LogTemp, Warning, TEXT("no clicked slot exist")); return; }
	if (EquipText->GetText().ToString().Equals("Equip"))
	{
		if (CurrentClickedSlot->GetItemType() == EItemType::Weapon)
		{
			ContextBarSwitcher->SetActiveWidgetIndex(1);
		}
		else
		{
			ContextBarSwitcher->SetActiveWidgetIndex(2);
		}
	}
	else
	{
		const FItemRuntimeInfo* TempItemData = InventoryComponent->GetItemData(CurrentClickedSlot->GetItemId());
		if (TempItemData == nullptr) { return; }
		if (CurrentClickedSlot->GetItemType() == EItemType::Weapon)
		{
			InventoryComponent->UnequipItemFromWeaponSlot(*TempItemData);
		}
		else
		{
			InventoryComponent->UnequipItemFromMiscSlot(*TempItemData);
		}
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

void UCPP_InventoryHUD::Slot1WeaponEquipFunc()
{
	const FItemRuntimeInfo* TempItemData = InventoryComponent->GetItemData(CurrentClickedSlot->GetItemId());
	if (TempItemData == nullptr) { return; }
	InventoryComponent->EquipItemToWeaponSlot(*TempItemData, 1);
	CloseContextMenu();
}

void UCPP_InventoryHUD::Slot2WeaponEquipFunc()
{
	const FItemRuntimeInfo* TempItemData = InventoryComponent->GetItemData(CurrentClickedSlot->GetItemId());
	if (TempItemData == nullptr) { return; }
	InventoryComponent->EquipItemToWeaponSlot(*TempItemData, 2);
	CloseContextMenu();
}

void UCPP_InventoryHUD::Slot3WeaponEquipFunc()
{
	const FItemRuntimeInfo* TempItemData = InventoryComponent->GetItemData(CurrentClickedSlot->GetItemId());
	if (TempItemData == nullptr) { return; }
	InventoryComponent->EquipItemToWeaponSlot(*TempItemData, 3);
	CloseContextMenu();
}

void UCPP_InventoryHUD::Slot4WeaponEquipFunc()
{
	const FItemRuntimeInfo* TempItemData = InventoryComponent->GetItemData(CurrentClickedSlot->GetItemId());
	if (TempItemData == nullptr) { return; }
	InventoryComponent->EquipItemToWeaponSlot(*TempItemData, 4);
	CloseContextMenu();
}

void UCPP_InventoryHUD::Slot1MiscEquipFunc()
{
	const FItemRuntimeInfo* TempItemData = InventoryComponent->GetItemData(CurrentClickedSlot->GetItemId());
	if (TempItemData == nullptr) { return; }
	InventoryComponent->EquipItemToMiscSlot(*TempItemData, 1);
	CloseContextMenu();
}
void UCPP_InventoryHUD::Slot2MiscEquipFunc()
{
	const FItemRuntimeInfo* TempItemData = InventoryComponent->GetItemData(CurrentClickedSlot->GetItemId());
	if (TempItemData == nullptr) { return; }
	InventoryComponent->EquipItemToMiscSlot(*TempItemData, 2);
	CloseContextMenu();
}
void UCPP_InventoryHUD::Slot3MiscEquipFunc()
{
	const FItemRuntimeInfo* TempItemData = InventoryComponent->GetItemData(CurrentClickedSlot->GetItemId());
	if (TempItemData == nullptr) { return; }
	InventoryComponent->EquipItemToMiscSlot(*TempItemData, 3);
	CloseContextMenu();
}
void UCPP_InventoryHUD::Slot4MiscEquipFunc()
{
	const FItemRuntimeInfo* TempItemData = InventoryComponent->GetItemData(CurrentClickedSlot->GetItemId());
	if (TempItemData == nullptr) { return; }
	InventoryComponent->EquipItemToMiscSlot(*TempItemData, 4);
	CloseContextMenu();
}
void UCPP_InventoryHUD::Slot5MiscEquipFunc()
{
	const FItemRuntimeInfo* TempItemData = InventoryComponent->GetItemData(CurrentClickedSlot->GetItemId());
	if (TempItemData == nullptr) { return; }
	InventoryComponent->EquipItemToMiscSlot(*TempItemData, 5);
	CloseContextMenu();
}
void UCPP_InventoryHUD::Slot6MiscEquipFunc()
{
	const FItemRuntimeInfo* TempItemData = InventoryComponent->GetItemData(CurrentClickedSlot->GetItemId());
	if (TempItemData == nullptr) { return; }
	InventoryComponent->EquipItemToMiscSlot(*TempItemData, 6);
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
		Slot1Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		WSlot1Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		if (ItemData.StaticData.ItemName.Equals("Empty"))
		{
			WSlot1Ammo->SetText(FText::FromString(""));
			WSlot1TotalAmmo->SetText(FText::FromString(""));
			Slot1Ammo->SetText(FText::FromString(""));
		}
		else
		{
			if (ItemData.StaticData.ItemType == EItemType::Weapon)
			{
				WSlot1Ammo->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(ItemData.WeaponData.MagSize)));
				WSlot1TotalAmmo->SetText(FText::Format(FText::FromString(TEXT("{0}")), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
				Slot1Ammo->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
			}
		}
		break;
	case 2:
		Slot2Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		WSlot2Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		if (ItemData.StaticData.ItemName.Equals("Empty"))
		{
			WSlot2Ammo->SetText(FText::FromString(""));
			WSlot2TotalAmmo->SetText(FText::FromString(""));
			Slot2Ammo->SetText(FText::FromString(""));
		}
		else
		{
			if (ItemData.StaticData.ItemType == EItemType::Weapon)
			{
				WSlot2Ammo->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(ItemData.WeaponData.MagSize)));
				WSlot2TotalAmmo->SetText(FText::Format(FText::FromString(TEXT("{0}")), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
				Slot2Ammo->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
			}
		}
		break;
	case 3:
		Slot3Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		WSlot3Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		if (ItemData.StaticData.ItemName.Equals("Empty"))
		{
			WSlot3Ammo->SetText(FText::FromString(""));
			WSlot3TotalAmmo->SetText(FText::FromString(""));
			Slot3Ammo->SetText(FText::FromString(""));
		}
		else
		{
			if (ItemData.StaticData.ItemType == EItemType::Weapon)
			{
				WSlot3Ammo->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(ItemData.WeaponData.MagSize)));
				WSlot3TotalAmmo->SetText(FText::Format(FText::FromString(TEXT("{0}")), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
				Slot3Ammo->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
			}
		}
		break;
	case 4:
		Slot4Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		WSlot4Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		if (ItemData.StaticData.ItemName.Equals("Empty"))
		{
			WSlot4Ammo->SetText(FText::FromString(""));
			WSlot4TotalAmmo->SetText(FText::FromString(""));
			Slot4Ammo->SetText(FText::FromString(""));
		}
		else
		{
			if (ItemData.StaticData.ItemType == EItemType::Weapon)
			{
				WSlot4Ammo->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(ItemData.WeaponData.MagSize)));
				WSlot4TotalAmmo->SetText(FText::Format(FText::FromString(TEXT("{0}")), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
				Slot4Ammo->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(ItemData.WeaponData.Ammos), FText::AsNumber(InventoryComponent->GetAmmoAmount(ItemData.WeaponData.AmmoType))));
			}
		}
		break;
	default:
		break;
	}
}

void UCPP_InventoryHUD::ChangeMSlot(int32 SlotNum, const FItemRuntimeInfo& ItemData)
{
	switch (SlotNum)
	{
	case 1:
		MSlot1Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		ISlot1Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		ISlot1Quantity->SetText(ItemData.ItemQuantity == 1 ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
		MSlot1Ammo->SetText(ItemData.StaticData.ItemType == EItemType::None ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
		break;
	case 2:
		MSlot2Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		ISlot2Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		ISlot2Quantity->SetText(ItemData.ItemQuantity == 1 ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
		MSlot2Ammo->SetText(ItemData.StaticData.ItemType == EItemType::None ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
		break;
	case 3:
		MSlot3Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		ISlot3Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		ISlot3Quantity->SetText(ItemData.ItemQuantity == 1 ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
		MSlot3Ammo->SetText(ItemData.StaticData.ItemType == EItemType::None ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
		break;
	case 4:
		MSlot4Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		ISlot4Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		ISlot4Quantity->SetText(ItemData.ItemQuantity == 1 ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
		MSlot4Ammo->SetText(ItemData.StaticData.ItemType == EItemType::None ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
		break;
	case 5:
		MSlot5Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		ISlot5Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		ISlot5Quantity->SetText(ItemData.ItemQuantity == 1 ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
		MSlot5Ammo->SetText(ItemData.StaticData.ItemType == EItemType::None ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
		break;
	case 6:
		MSlot6Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		ISlot6Text->SetText(FText::FromString(ItemData.StaticData.ItemName));
		ISlot6Quantity->SetText(ItemData.ItemQuantity == 1 ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
		MSlot6Ammo->SetText(ItemData.StaticData.ItemType == EItemType::None ? FText::FromString("") : FText::AsNumber(ItemData.ItemQuantity));
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

void UCPP_InventoryHUD::MDelayOnSelected()
{
	bIsMSlotSelected = false;
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
	InventoryComponent->OnMSlotChanged.AddDynamic(this, &UCPP_InventoryHUD::ChangeMSlot);
	InventoryComponent->OnItemRemoved.AddDynamic(this, &UCPP_InventoryHUD::OnRemovedItem);
	InventoryComponent->OnAmmoChanged.AddDynamic(this, &UCPP_InventoryHUD::ChangeAmmo);
	InventoryComponent->OnItemEquipped.AddDynamic(this, &UCPP_InventoryHUD::EquipItem);

	InitializeInventory();

	if (ContextMenu)
	{
		ContextMenuCanvasSlot = Cast<UCanvasPanelSlot>(ContextMenu->Slot);
	}

	EquipButton->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::OnEquipButtonClicked);
	Slot1WeaponEquip->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::Slot1WeaponEquipFunc);
	Slot2WeaponEquip->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::Slot2WeaponEquipFunc);
	Slot3WeaponEquip->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::Slot3WeaponEquipFunc);
	Slot4WeaponEquip->OnClicked.AddDynamic(this, &UCPP_InventoryHUD::Slot4WeaponEquipFunc);
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
			WeaponSelector->SetVisibility(ESlateVisibility::Hidden);
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

	if (ClickedSlot->GetItemType() == EItemType::Weapon)
	{
		if (!InventoryComponent->IsWeaponEquipped(ClickedSlot->GetInstanceID()))
		{
			EquipText->SetText(FText::FromString("Equip"));
		}
		else
		{
			EquipText->SetText(FText::FromString("Unequip"));
		}
	}
	else if (ClickedSlot->GetItemType() == EItemType::Meds || ClickedSlot->GetItemType() == EItemType::Armor || ClickedSlot->GetItemType() == EItemType::Utility)
	{
		if (!InventoryComponent->IsMiscEquipped(ClickedSlot->GetInstanceID()))
		{
			EquipText->SetText(FText::FromString("Equip"));
		}
		else
		{
			EquipText->SetText(FText::FromString("Unequip"));
		}
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
