// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_InventoryComponent.h"
#include "NullZone/SpawnItems/CPP_ItemManager.h"
#include "Kismet/GameplayStatics.h"
#include "NullZone/SpawnItems/Actors/CPP_PickupActor.h"
#include "NullZone/SpawnItems/Actors/CPP_EquipActor.h"
#include "CPP_PlayerCharacter.h"

UCPP_InventoryComponent::UCPP_InventoryComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}


void UCPP_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCPP_InventoryComponent::EquipItemToSlot(const FItemRuntimeInfo& Item, int32 SlotToEquip)
{
	if (SlotToEquip == -1)
	{
		if (IsItemEquipped(Item.InstanceId))
		{
			OnSlotChanged.Broadcast(FindEquippedItemIndex(Item.InstanceId)+1, Item);
			return;
		}
		for (int32 i = 0; i < Slots.Num(); i++)
		{
			if (Slots[i].ItemName == "Empty")
			{
				Slots[i].ItemName = Item.StaticData.ItemName;
				Slots[i].InstanceId = Item.InstanceId;
				OnSlotChanged.Broadcast(i + 1, Item);
				if (CurrentSlot == i)
				{
					EquipSlot(i);
				}
				break;
			}
		}
	}
	else
	{
		Slots[SlotToEquip-1].ItemName = Item.StaticData.ItemName;
		Slots[SlotToEquip-1].InstanceId = Item.InstanceId;
		OnSlotChanged.Broadcast(SlotToEquip, Item);
	}
}

void UCPP_InventoryComponent::UnequipItemFromSlot(const FItemRuntimeInfo& Item)
{
	if (IsItemEquipped(Item.InstanceId))
	{
		int32 SlotIndex = FindEquippedItemIndex(Item.InstanceId);
		if (SlotIndex == -1) { return; }
		Slots[SlotIndex].ItemName = "Empty";
		Slots[SlotIndex].InstanceId = FGuid::NewGuid();
		OnSlotChanged.Broadcast(SlotIndex + 1, FItemRuntimeInfo());
	}
}



int32 UCPP_InventoryComponent::AddItem(const ACPP_PickupActor* NewActor)
{
	FItemRuntimeInfo NewItem = NewActor->GetRuntimeInfo();
	//UE_LOG(LogTemp, Warning, TEXT("Item InstanceId: %s"), *NewItem.InstanceId.ToString());
	if (NewItem.StaticData.ItemType == EItemType::Ammo)
	{
		if (Ammos.Find(NewItem.AmmoData.AmmoType))
		{
			Ammos[NewItem.AmmoData.AmmoType] += NewItem.ItemQuantity;
			OnAmmoChanged.Broadcast(NewItem.AmmoData.AmmoType, Ammos[NewItem.AmmoData.AmmoType]);
			ItemManager->SpawnItemInWorld(NewActor, NewItem, EItemState::Inventory, ItemManager->GetDefaultPoolPlace());
			bool flag = false;
			for (FItemRuntimeInfo& Item : Items)
			{
				if (Item.StaticData.ItemId == NewItem.StaticData.ItemId)
				{
					Item.ItemQuantity = Ammos[NewItem.AmmoData.AmmoType];
					Item.ItemState = EItemState::Inventory;
					flag = true;
					break;
				}
			}
			if (!flag)
			{
				Items.Add(NewItem);
			}
			for (int32 i = 0; i < 4; i++)
			{
				if (Slots[i].ItemName != "Empty")
				{
					for (FItemRuntimeInfo& Item : Items)
					{
						if (Item.InstanceId == Slots[i].InstanceId)
						{
							OnSlotChanged.Broadcast(i+1, Item);
						}
					}
				}
			}
			if (EquippedItem && EquippedItem->GetRuntimeInfo().StaticData.ItemType == EItemType::Weapon && EquippedItem->GetRuntimeInfo().WeaponData.AmmoType == NewItem.AmmoData.AmmoType)
			{
				OnItemEquipped.Broadcast(EquippedItem->GetRuntimeInfo());
			}
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		float NewItemWeight = NewItem.ItemWeight * (float)NewItem.ItemQuantity;
		for (FItemRuntimeInfo& Item : Items)
		{
			if (Item.StaticData.ItemId == NewItem.StaticData.ItemId && Item.StaticData.ItemType == NewItem.StaticData.ItemType)
			{
				if (NewItemWeight + Weight > MaxWeight)
				{
					return 0;
				}
				if (Item.StaticData.bIsStackable == false)
				{
					continue;
				}
				Item.ItemQuantity += NewItem.ItemQuantity;
				Weight += NewItemWeight;
				EquipItemToSlot(Item, -1);
				OnItemAdded.Broadcast(Item);
				ItemManager->SpawnItemInWorld(NewActor, NewItem, EItemState::Inventory, ItemManager->GetDefaultPoolPlace());
				Item.ItemState = EItemState::Inventory;
				if (EquippedItem && EquippedItem->GetRuntimeInfo().StaticData.ItemId == Item.StaticData.ItemId)
				{
					EquippedItem->SetRuntimeData(Item);
					OnItemEquipped.Broadcast(EquippedItem->GetRuntimeInfo());
				}
				return 1;
			}
		}
		if (Weight + NewItemWeight > MaxWeight)
		{
			return 0;
		}
		Items.Add(NewItem);
		Weight += NewItemWeight;
		EquipItemToSlot(NewItem, -1);
		OnItemAdded.Broadcast(NewItem);
		ItemManager->SpawnItemInWorld(NewActor, NewItem, EItemState::Inventory, ItemManager->GetDefaultPoolPlace());
		NewItem.ItemState = EItemState::Inventory;
		return 1;
	}
}

bool UCPP_InventoryComponent::RemoveItem(const FName& ItemId, int32 Quantity)
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].StaticData.ItemId == ItemId)
		{
			if (Items[i].ItemQuantity < Quantity)
				return false;
			FItemRuntimeInfo TempInfo;
			TempInfo = Items[i];
			TempInfo.ItemQuantity = Quantity;
			ItemManager->SpawnItemInWorld(nullptr, TempInfo, EItemState::Level, GetSpawnTransform());
			Items[i].ItemQuantity -= Quantity;
			OnItemRemoved.Broadcast(ItemId, Quantity);
			if (IsItemEquipped(Items[i].InstanceId))
			{
				OnSlotChanged.Broadcast(FindEquippedItemIndex(Items[i].InstanceId)+1, Items[i]);
			}
			if (EquippedItem && EquippedItem->GetRuntimeInfo().InstanceId == Items[i].InstanceId)
			{
				EquippedItem->SetRuntimeData(Items[i]);
			}
			if (Items[i].ItemQuantity == 0)
			{
				UnequipItemFromSlot(Items[i]);
				if (EquippedItem && EquippedItem->GetRuntimeInfo().InstanceId == Items[i].InstanceId)
				{
					UE_LOG(LogTemp, Warning, TEXT("item null"))
					EquippedItem = nullptr;
				}
				Items.RemoveAt(i);
			}
			return true;
		}
	}
	return false;
}

FTransform UCPP_InventoryComponent::GetSpawnTransform()
{
	AActor* Player = GetOwner();
	if (!Player) return FTransform::Identity;
	FVector Location = Player->GetActorLocation() + (Player->GetActorForwardVector() * 200.f);
	Location.Z -= 100.f;
	return FTransform(FRotator::ZeroRotator, Location, FVector::OneVector);
}


void UCPP_InventoryComponent::RemoveAmmo(const EAmmoType& AmmoType)
{
	if (!Ammos.Find(AmmoType)) return;
	if (Ammos[AmmoType] == 0) return;
	
	TArray<FName> RowNames = ItemDataTable->GetRowNames();
	FItemData* AmmoData = ItemDataTable->FindRow<FItemData>(AmmoIds[AmmoType], TEXT(""));
	for (FItemRuntimeInfo& Item : Items)
	{
		if (Item.StaticData.ItemType == EItemType::Ammo && Item.AmmoData.AmmoType == AmmoType)
		{
			Ammos[AmmoType] = 0;
			Item.ItemState = EItemState::Inventory;
			ItemManager->SpawnItemInWorld(nullptr, Item, EItemState::Level, GetSpawnTransform());
			Item.ItemState = EItemState::Level;
			Item.ItemQuantity = Ammos[AmmoType];
			break;
		}
	}
	OnAmmoChanged.Broadcast(AmmoType, Ammos[AmmoType]);
}


FItemRuntimeInfo* UCPP_InventoryComponent::FindItem(const FName& ItemId)
{
	for (FItemRuntimeInfo& Item : Items)
	{
		if (Item.StaticData.ItemId == ItemId)
		{
			return &Item;
		}
	}
	return nullptr;
}

float UCPP_InventoryComponent::GetTotalWeight() const
{
	float TotalWeight = 0.0f;
	for (const FItemRuntimeInfo& Item : Items)
	{
		TotalWeight += Item.ItemWeight * Item.ItemQuantity;
	}
	return TotalWeight;
}

void UCPP_InventoryComponent::InitializeInventory(UDataTable* TempItemDataTable)
{
	Weight = 0.f;
	MaxWeight = 100.f;
	ItemDataTable = TempItemDataTable;
	Character = Cast<ACPP_PlayerCharacter>(GetOwner());
	TArray<EAmmoType> AmmoEnums = { 
	EAmmoType::CompactAmmo,
	EAmmoType::MachinePistol,
	EAmmoType::ServiceAmmo,
	EAmmoType::IntermediateHV,
	EAmmoType::PrecisionAmmo,
	EAmmoType::LongRangeAmmo,
	EAmmoType::ShotgunShell,
	EAmmoType::HighCaliberSurplus };
	for (EAmmoType AmmoType : AmmoEnums)
	{
		Ammos.Add(AmmoType, 0);
	}
	AmmoNames.Add(EAmmoType::CompactAmmo, "Compact Ammo");
	AmmoNames.Add(EAmmoType::MachinePistol, "Machine Pistol Ammo");
	AmmoNames.Add(EAmmoType::ServiceAmmo, "Service Ammo");
	AmmoNames.Add(EAmmoType::IntermediateHV, "Intermediate HV");
	AmmoNames.Add(EAmmoType::PrecisionAmmo, "Precision Ammo");
	AmmoNames.Add(EAmmoType::LongRangeAmmo, "Long Range Ammo");
	AmmoNames.Add(EAmmoType::ShotgunShell, "Shotgun Shell");
	AmmoNames.Add(EAmmoType::HighCaliberSurplus, "High Caliber Surplus");

	AmmoIds.Add(EAmmoType::CompactAmmo, FName("CompactAmmo"));
	AmmoIds.Add(EAmmoType::MachinePistol, FName("MachinePistolAmmo"));
	AmmoIds.Add(EAmmoType::ServiceAmmo, FName("ServiceAmmo"));
	AmmoIds.Add(EAmmoType::IntermediateHV, FName("IntermediateHV"));
	AmmoIds.Add(EAmmoType::PrecisionAmmo, FName("PrecisionAmmo"));
	AmmoIds.Add(EAmmoType::LongRangeAmmo, FName("LongRangeAmmo"));
	AmmoIds.Add(EAmmoType::ShotgunShell, FName("ShotgunShell"));
	AmmoIds.Add(EAmmoType::HighCaliberSurplus, FName("HighCaliberSurplus"));

	for (int i = 0; i < 6; i++)
	{
		FSlotData Slot;
		Slot.ItemName = "Empty";
		Slot.InstanceId = FGuid::NewGuid();
		Slots.Add(Slot);
		OnSlotChanged.Broadcast(i + 1, FItemRuntimeInfo());
	}

	OnItemEquipped.Broadcast(FItemRuntimeInfo());
	ItemManager = Cast<ACPP_ItemManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACPP_ItemManager::StaticClass()));
	if (!ItemManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get ItemManager"));
	}
}

void UCPP_InventoryComponent::EquipSlot(int32 Slot)
{
	if (Slot > Slots.Num() || Slot == -1) return;
	if (EquippedItem)
	{
		EquippedItem->ChangedItem();
		ItemManager->UnequipItemFromPlayer();
	}
	CurrentSlot = Slot;
	if (Slot < 0)
	{
		EquippedItem = nullptr;
		OnItemEquipped.Broadcast(FItemRuntimeInfo());
		return;
	}
	else if (Slots[Slot].ItemName != "Empty")
	{
		for (FItemRuntimeInfo Data : Items)
		{
			if (Data.InstanceId == Slots[Slot].InstanceId)
			{
				EquippedItem = ItemManager->EquipItemToPlayer(Data, Character);
				if (!IsValid(EquippedItem)) return;
				OnItemEquipped.Broadcast(EquippedItem->GetRuntimeInfo());
				break;
			}
		}
		
	}
	else
	{
		EquippedItem = nullptr;
		OnItemEquipped.Broadcast(FItemRuntimeInfo());
	}
}

void UCPP_InventoryComponent::UseItem(const FName& ItemId)
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].StaticData.ItemId == ItemId)
		{
			Items[i].ItemQuantity -= 1;
			OnItemRemoved.Broadcast(ItemId, 1);
			if (IsItemEquipped(Items[i].InstanceId))
			{
				OnSlotChanged.Broadcast(FindEquippedItemIndex(Items[i].InstanceId) + 1, Items[i]);
			}
			if (EquippedItem && EquippedItem->GetRuntimeInfo().InstanceId == Items[i].InstanceId)
			{
				EquippedItem->SetRuntimeData(Items[i]);
			}
			if (Items[i].ItemQuantity == 0)
			{
				UnequipItemFromSlot(Items[i]);
				if (EquippedItem && EquippedItem->GetRuntimeInfo().InstanceId == Items[i].InstanceId)
				{
					EquippedItem = nullptr;
				}
				Items.RemoveAt(i);
				OnItemEquipped.Broadcast(FItemRuntimeInfo());
			}
			else
			{
				OnItemEquipped.Broadcast(EquippedItem->GetRuntimeInfo());
			}
			return;
		}
	}
}

void UCPP_InventoryComponent::FireWeapon()
{
	
	FItemRuntimeInfo WeaponRuntimeData = EquippedItem->GetRuntimeInfo();
	if (WeaponRuntimeData.WeaponData.Ammos > 0)
	{
		for (FItemRuntimeInfo& Item : Items)
		{
			if (Item.InstanceId == WeaponRuntimeData.InstanceId)
			{
				Item.WeaponData.Ammos--;
				EquippedItem->SetRuntimeData(Item);
				break;
			}
		}
		for (int32 i = 0; i < 4; i++)
		{
			if (Slots[i].ItemName != "Empty")
			{
				for (FItemRuntimeInfo& Item : Items)
				{
					if (Item.InstanceId == Slots[i].InstanceId)
					{
						OnSlotChanged.Broadcast(i + 1, Item);
					}
				}
			}
		}
		OnItemEquipped.Broadcast(EquippedItem->GetRuntimeInfo());
	}
}

bool UCPP_InventoryComponent::ReloadWeapon()
{
	if (EquippedItem && EquippedItem->GetRuntimeInfo().StaticData.ItemType == EItemType::Weapon)
	{
		FItemRuntimeInfo WeaponRuntimeData = EquippedItem->GetRuntimeInfo();
		if (WeaponRuntimeData.WeaponData.Ammos >= WeaponRuntimeData.WeaponData.MagSize) return false;
		for (FItemRuntimeInfo& Item : Items)
		{
			if (Item.InstanceId == WeaponRuntimeData.InstanceId)
			{
				int32 reqAmount = Item.WeaponData.MagSize - Item.WeaponData.Ammos;
				int32 AvailabelAmount = FMath::Min(reqAmount, Ammos[WeaponRuntimeData.WeaponData.AmmoType]);
				if (AvailabelAmount == 0) return false;
				Item.WeaponData.Ammos += AvailabelAmount;
				Ammos[WeaponRuntimeData.WeaponData.AmmoType] -= AvailabelAmount;
				EquippedItem->SetRuntimeData(Item);
				OnAmmoChanged.Broadcast(WeaponRuntimeData.WeaponData.AmmoType, Ammos[WeaponRuntimeData.WeaponData.AmmoType]);
				OnItemEquipped.Broadcast(EquippedItem->GetRuntimeInfo());
				return true;
			}
		}
	}
	return false;
}

bool UCPP_InventoryComponent::UnloadWeapon()
{
	if (EquippedItem && EquippedItem->GetRuntimeInfo().StaticData.ItemType == EItemType::Weapon)
	{
		FItemRuntimeInfo WeaponRuntimeData = EquippedItem->GetRuntimeInfo();
		if (WeaponRuntimeData.WeaponData.Ammos == 0) return false;
		for (FItemRuntimeInfo& Item : Items)
		{
			if (Item.InstanceId == WeaponRuntimeData.InstanceId)
			{
				Ammos[WeaponRuntimeData.WeaponData.AmmoType] += Item.WeaponData.Ammos;
				Item.WeaponData.Ammos = 0;
				EquippedItem->SetRuntimeData(Item);
				OnAmmoChanged.Broadcast(WeaponRuntimeData.WeaponData.AmmoType, Ammos[WeaponRuntimeData.WeaponData.AmmoType]);
				OnItemEquipped.Broadcast(EquippedItem->GetRuntimeInfo());
				return true;
			}
		}
	}
	return false;
}
