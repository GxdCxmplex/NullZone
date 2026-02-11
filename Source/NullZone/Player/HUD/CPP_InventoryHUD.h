// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NullZone/Data/ItemInfo.h"
#include "CPP_InventoryHUD.generated.h"


class UImage;
class UButton;
class UBorder;
class UCanvasPanel;
class UWidgetAnimation;
class UTextBlock;
class UWidgetSwitcher;
class UCanvasPanelSlot;
class UCPP_InventoryComponent;

/**
 * 
 */


UCLASS()
class NULLZONE_API UCPP_InventoryHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	void LookEvent(float MouseX, float MouseY);
	void MLookEvent(float MouseX, float MouseY);

	void ResetSlot(int32 CurrentSlotTemp);
	void ResetMSlot(int32 CurrentSlotTemp);

	void VisibilityHandler(bool bVisibility);
	void MVisibilityHandler(bool bVisibility);

	void Slot1Select(bool Selected);
	void Slot2Select(bool Selected);
	void Slot3Select(bool Selected);
	void Slot4Select(bool Selected);

	void MSlot1Select(bool Selected);
	void MSlot2Select(bool Selected);
	void MSlot3Select(bool Selected);
	void MSlot4Select(bool Selected);
	void MSlot5Select(bool Selected);
	void MSlot6Select(bool Selected);
	
	UFUNCTION()
	void OnEquipButtonClicked();

	UFUNCTION()
	void OnDrop1ButtonClicked();
	UFUNCTION()
	void OnDropAllButtonClicked();

	UFUNCTION()
	void Slot1WeaponEquipFunc();
	UFUNCTION()
	void Slot2WeaponEquipFunc();
	UFUNCTION()
	void Slot3WeaponEquipFunc();
	UFUNCTION()
	void Slot4WeaponEquipFunc();

	UFUNCTION()
	void Slot1MiscEquipFunc();
	UFUNCTION()
	void Slot2MiscEquipFunc();
	UFUNCTION()
	void Slot3MiscEquipFunc();
	UFUNCTION()
	void Slot4MiscEquipFunc();
	UFUNCTION()
	void Slot5MiscEquipFunc();
	UFUNCTION()
	void Slot6MiscEquipFunc();

	UFUNCTION()
	void CompactAmmoDropFunc();
	UFUNCTION()
	void MachinePistolDropFunc();
	UFUNCTION()
	void ServiceAmmoDropFunc();
	UFUNCTION()
	void IntermediateHVDropFunc();
	UFUNCTION()
	void PrecisionAmmoDropFunc();
	UFUNCTION()
	void LongRangeAmmoDropFunc();
	UFUNCTION()
	void ShotgunShellDropFunc();
	UFUNCTION()
	void HighCaliberSurplusDropFunc();

	UFUNCTION()
	void ChangeSlot(int32 SlotNum, const FItemRuntimeInfo& ItemData);
	UFUNCTION()
	void ChangeMSlot(int32 SlotNum, const FItemRuntimeInfo& ItemData);
	UFUNCTION()
	void ChangeAmmo(const EAmmoType& AmmoType, const int32& Quantity);

	UFUNCTION()
	void EquipItem(const FItemRuntimeInfo& ItemInfo);

	void InitializeInventory();

	void OnInventoryButtonClicked(bool DoWhat);

	void CloseContextMenu();

	UFUNCTION()
	void HandleInventorySlotClicked(UCPP_InventorySlot* ClickedSlot);

	UFUNCTION()
	void OnAddedItem(const FItemRuntimeInfo& Items);
	UFUNCTION()
	void OnRemovedItem(FName ItemID, int32 Quanity);

	UFUNCTION()
	void DelayOnSelected();

	UFUNCTION()
	void MDelayOnSelected();

	int32 GetCurrentSlot();
	int32 GetCurrentMSlot();

protected:
	
	virtual void NativeConstruct() override;
	
protected:

	//weapon wheel
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Slot1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Slot1Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Slot1Ammo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Slot2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Slot2Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Slot2Ammo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Slot3;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Slot3Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Slot3Ammo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Slot4;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Slot4Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Slot4Ammo;

	//misc wheel
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* MSlot1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MSlot1Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MSlot1Ammo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* MSlot2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MSlot2Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MSlot2Ammo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* MSlot3;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MSlot3Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MSlot3Ammo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* MSlot4;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MSlot4Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MSlot4Ammo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* MSlot5;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MSlot5Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MSlot5Ammo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* MSlot6;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MSlot6Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MSlot6Ammo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* WeaponSelector;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* MiscWheel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* InventoryCanvas;

	//weapon Wheel animations
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Slot1Selected;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Slot2Selected;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Slot3Selected;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Slot4Selected;

	//misc wheel animations
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* MSlot1Selected;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* MSlot2Selected;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* MSlot3Selected;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* MSlot4Selected;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* MSlot5Selected;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* MSlot6Selected;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* ContextMenu;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* ContextBarSwitcher;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* EquipButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Drop1Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* DropAllButton;

	//weapon equip
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Slot1WeaponEquip;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Slot2WeaponEquip;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Slot3WeaponEquip;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Slot4WeaponEquip;

	//misc equip
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Slot1MiscEquip;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Slot2MiscEquip;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Slot3MiscEquip;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Slot4MiscEquip;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Slot5MiscEquip;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Slot6MiscEquip;

	//Weapon slots
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WSlot1Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WSlot1Ammo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WSlot1TotalAmmo;
	//
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WSlot2Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WSlot2Ammo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WSlot2TotalAmmo;
	//
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WSlot3Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WSlot3Ammo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WSlot3TotalAmmo;
	//
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WSlot4Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WSlot4Ammo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WSlot4TotalAmmo;

	//Misc slot frame
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ISlot1Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ISlot1Quantity;
	//
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ISlot2Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ISlot2Quantity;
	//
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ISlot3Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ISlot3Quantity;
	//
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ISlot4Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ISlot4Quantity;
	//
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ISlot5Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ISlot5Quantity;
	//
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ISlot6Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ISlot6Quantity;

	// Ammo Frame
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CompactAmmoAmount;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* CompactAmmoDrop;
	//
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MachinePistolAmount;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* MachinePistolDrop;
	//
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ServiceAmmoAmount;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ServiceAmmoDrop;
	//
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* IntermediateHVAmount;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* IntermediateHVDrop;
	//
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PrecisionAmmoAmount;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* PrecisionAmmoDrop;
	//
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* LongRangeAmount;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* LongRangeDrop;
	//
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ShotgunShellAmount;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ShotgunShellDrop;
	//
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* HighCaliberSurplusAmount;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* HighCaliberSurplusDrop;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* EquipText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* EquippedSlotText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* EquippedSlotTotalAmmo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* EquippedSlotAmmo;

	UPROPERTY()
	UCanvasPanelSlot* ContextMenuCanvasSlot;

	bool bIsSlotSelected = false;
	bool bIsMSlotSelected = false;
	bool bIsContextMenuVisible = false;
	
	int32 CurrentSlot = -1;
	int32 LastSlot = -1;
	bool bSlotChosen = false;

	int32 MCurrentSlot = -1;
	int32 MLastSlot = -1;
	bool bMSlotChosen = false;

	FTimerHandle SelectedDelay;

	FTimerHandle MSelectedDelay;

	UPROPERTY()
	UCPP_InventorySlot* CurrentClickedSlot = nullptr;

	UPROPERTY()
	class UCPP_InventoryGrid* InventoryGrid;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TSubclassOf<UCPP_InventoryGrid> InventoryGridClass;

	UPROPERTY()
	UCPP_InventoryComponent* InventoryComponent;

};
