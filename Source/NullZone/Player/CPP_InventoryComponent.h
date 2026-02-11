// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NullZone/Data/ItemInfo.h"
#include "CPP_InventoryComponent.generated.h"

class ACPP_ItemManager;
class ACPP_PickupActor;
class ACPP_EquipActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAdded, const FItemRuntimeInfo&, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemEquipped, const FItemRuntimeInfo&, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotChanged, int32, Slot, const FItemRuntimeInfo&, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMSlotChanged, int32, Slot, const FItemRuntimeInfo&, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemRemoved, FName, ItemId, int32, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, const EAmmoType&, Ammo, const int32&, Quantity);

USTRUCT(BlueprintType)
struct FSlotData
{
	GENERATED_BODY()

	UPROPERTY()
	FString ItemName;

	UPROPERTY()
	FGuid InstanceId;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NULLZONE_API UCPP_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPP_InventoryComponent();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 CurrentSlot = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 MCurrentSlot = -1;

	ACPP_EquipActor* EquippedItem = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FSlotData> WeaponSlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FSlotData> MiscSlots;


public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FItemRuntimeInfo> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<EAmmoType, int32> Ammos;

	TMap<EAmmoType, FString> AmmoNames;

	TMap<EAmmoType, FName> AmmoIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float MaxWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float Weight;

	UPROPERTY(BlueprintAssignable)
	FOnItemAdded OnItemAdded;

	UPROPERTY(BlueprintAssignable)
	FOnSlotChanged OnSlotChanged;

	UPROPERTY(BlueprintAssignable)
	FOnMSlotChanged OnMSlotChanged;

	UPROPERTY(BlueprintAssignable)
	FOnItemRemoved OnItemRemoved;

	UPROPERTY(BlueprintAssignable)
	FOnItemEquipped OnItemEquipped;

	UPROPERTY(BlueprintAssignable)
	FOnAmmoChanged OnAmmoChanged;

	int32 AddItem(const ACPP_PickupActor* NewActor);

	bool RemoveItem(const FName& ItemId, int32 Quantity);

	FTransform GetSpawnTransform();

	void RemoveAmmo(const EAmmoType& AmmoType);

	FItemRuntimeInfo* FindItem(const FName& ItemId);

	float GetTotalWeight() const;

	void EquipItemToWeaponSlot(const FItemRuntimeInfo& Item, int32 SlotToEquip);
	void UnequipItemFromWeaponSlot(const FItemRuntimeInfo& Item);

	void EquipItemToMiscSlot(const FItemRuntimeInfo& Item, int32 SlotToEquip);
	void UnequipItemFromMiscSlot(const FItemRuntimeInfo& Item);

	void InitializeInventory(UDataTable* TempItemDataTable);
	void EquipWeaponSlot(int32 Slot);
	void EquipMiscSlot(int32 Slot);

	void UseItem(const FName& ItemId);
	void FireWeapon();
	bool ReloadWeapon();
	bool UnloadWeapon();
	
	FORCEINLINE const bool IsWeaponEquipped(const FGuid& IdToFind) const
	{
		bool Flag = false;
		for (FSlotData Slot : WeaponSlots)
		{
			if (Slot.InstanceId == IdToFind)
			{
				return true;
			}
		}
		return false;
	}

	FORCEINLINE const bool IsMiscEquipped(const FGuid& IdToFind) const
	{
		bool Flag = false;
		for (FSlotData Slot : MiscSlots)
		{
			if (Slot.InstanceId == IdToFind)
			{
				return true;
			}
		}
		return false;
	}

	FORCEINLINE const FItemRuntimeInfo* GetItemData(const FName& IdToFind) const
	{
		for (const FItemRuntimeInfo& Item : Items)
		{
			if (IdToFind == Item.StaticData.ItemId)
			{
				return &Item;
			}
		}
		return nullptr;
	}

	FORCEINLINE int32 GetAmmoAmount(EAmmoType Type)
	{
		if (!Ammos[Type]) return 0;
		return Ammos[Type];
	}

	FORCEINLINE ACPP_EquipActor* GetEquippedActor() const { return EquippedItem; }

private:

	ACharacter* Character;

	UPROPERTY()
	ACPP_ItemManager* ItemManager;

	UPROPERTY()
	UDataTable* ItemDataTable;

	int32 FindEquippedWeaponIndex(const FGuid& IdToFind)
	{
		for (int32 i = 0; i < WeaponSlots.Num(); i++)
		{
			if (WeaponSlots[i].InstanceId == IdToFind)
			{
				return i;
			}
		}
		return -1;
	}

	int32 FindEquippedMiscIndex(const FGuid& IdToFind)
	{
		for (int32 i = 0; i < MiscSlots.Num(); i++)
		{
			if (MiscSlots[i].InstanceId == IdToFind)
			{
				return i;
			}
		}
		return -1;
	}
};

