// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ObjectPoolManager.h"
#include "Actors/CPP_EquipActor.h"
#include "Actors/CPP_PickupActor.h"
#include "NullZone/Interactable.h"

UCPP_ObjectPoolManager::UCPP_ObjectPoolManager()
{
	ItemsPoolSizes.Add("AR4", 5);
}

void UCPP_ObjectPoolManager::InitializePools(UDataTable* TempItemDataTable, TSubclassOf<ACPP_EquipActor> EquipClass, TSubclassOf<ACPP_PickupActor> PickupClass)
{
	ItemDataTable = TempItemDataTable;
	EquipActor = EquipClass;
	PickupActor = PickupClass;
	if (!ItemDataTable || !EquipActor || !PickupActor) return;
	TArray<FName> RowNames = ItemDataTable->GetRowNames();

	for (const FName& ItemID : RowNames)
	{
		FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemID, "");
		if (!ItemData) continue;
		int32 PoolSize = ItemsPoolSizes.FindRef(ItemID);
		if (PoolSize <= 0) PoolSize = 5;
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		TArray<ACPP_PickupActor*> ItemPickupPool;
		for (int32 i = 0; i < PoolSize; i++)
		{
			ACPP_PickupActor* Actor = GetWorld()->SpawnActor<ACPP_PickupActor>(PickupActor, FVector(500, 500, 100), FRotator::ZeroRotator, SpawnParams);
			Actor->Initialize(*ItemData);
			Actor->SetActorHiddenInGame(true);
			Actor->SetActorEnableCollision(false);
			Actor->SetActorTickEnabled(false);

			ItemPickupPool.Add(Actor);
			
		}
		AllPickupActors.Add(ItemID, ItemPickupPool);
		if (ItemData->ItemType == EItemType::Weapon || ItemData->ItemType == EItemType::Meelee || ItemData->ItemType == EItemType::Meds)
		{
			TArray<ACPP_EquipActor*> ItemEquipPool;
			for (int32 i = 0; i < PoolSize; i++)
			{
				ACPP_EquipActor* Actor = GetWorld()->SpawnActor<ACPP_EquipActor>(ACPP_EquipActor::StaticClass(), FVector(500, 500, 100), FRotator::ZeroRotator, SpawnParams);
				
				Actor->Initialize(*ItemData);
				Actor->SetActorHiddenInGame(true);
				Actor->SetActorEnableCollision(false);
				Actor->SetActorTickEnabled(false);

				ItemEquipPool.Add(Actor);
			}
			AllEquipActors.Add(ItemID, ItemEquipPool);
		}
	}
}

ACPP_EquipActor* UCPP_ObjectPoolManager::GetEquipActor(const FItemRuntimeInfo& ItemInfo)
{
	if (!AllEquipActors.Contains(ItemInfo.StaticData.ItemId)) return nullptr;

	for (ACPP_EquipActor* Actor : AllEquipActors[ItemInfo.StaticData.ItemId])
	{
		if (Actor->IsHidden() && !Actor->IsActorBeingDestroyed() && Actor != CurrentEquipActor)
		{
			Actor->SetActorHiddenInGame(false);
			Actor->SetActorEnableCollision(true);
			Actor->SetActorTickEnabled(true);
			Actor->SetRuntimeData(ItemInfo);
			Actor->ResetToPoolState();
			CurrentEquipActor = Actor;
			AllEquipActors[ItemInfo.StaticData.ItemId].RemoveAtSwap(0);
			return Actor;
		}
	}

	FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemInfo.StaticData.ItemId, "");
	if (!ItemData) return nullptr;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ACPP_EquipActor* NewActor = GetWorld()->SpawnActor<ACPP_EquipActor>(ACPP_EquipActor::StaticClass(), FVector(0, 0, 0), FRotator::ZeroRotator, SpawnParams);
	NewActor->Initialize(*ItemData);
	//AllEquipActors.FindOrAdd(ItemInfo.StaticData.ItemId).Add(NewActor);
	CurrentEquipActor = NewActor;
	return NewActor;
}

ACPP_EquipActor* UCPP_ObjectPoolManager::GetEquipActorForNpc(const FName& ItemId)
{
	if (!AllEquipActors.Contains(ItemId)) return nullptr;

	for (ACPP_EquipActor* Actor : AllEquipActors[ItemId])
	{
		if (Actor->IsHidden() && !Actor->IsActorBeingDestroyed() && Actor != CurrentEquipActor)
		{
			Actor->SetActorHiddenInGame(false);
			Actor->SetActorEnableCollision(true);
			Actor->SetActorTickEnabled(true);
			Actor->ResetRuntimeData();
			Actor->ResetToPoolState();
			CurrentEquipActor = Actor;
			AllEquipActors[ItemId].RemoveAtSwap(0);
			return Actor;
		}
	}
	FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemId, "");
	if (!ItemData) return nullptr;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ACPP_EquipActor* NewActor = GetWorld()->SpawnActor<ACPP_EquipActor>(ACPP_EquipActor::StaticClass(), FVector(0, 0, 0), FRotator::ZeroRotator, SpawnParams);
	NewActor->Initialize(*ItemData);
	//AllEquipActors.FindOrAdd(ItemId).Add(NewActor);
	CurrentEquipActor = NewActor;
	return NewActor;
}

ACPP_PickupActor* UCPP_ObjectPoolManager::GetPickupActor(const ACPP_PickupActor* NewItem, const FItemRuntimeInfo& ItemData, EItemState NewState)
{
	TArray<ACPP_PickupActor*>& Pool = GetPool(ItemData.ItemState, ItemData.StaticData.ItemId);

	for (int32 i = 0; i < Pool.Num(); ++i)
	{
		ACPP_PickupActor* Actor = Pool[i];
		if (!Actor || Actor->IsActorBeingDestroyed()) continue;
		if (NewItem)
		{
			if (Actor == NewItem)
			{
				Pool.RemoveAtSwap(i);
				Actor->ResetToPoolState();
				Actor->SetActorHiddenInGame(true);
				Actor->SetActorEnableCollision(false);
				Actor->SetActorTickEnabled(false);

				if (!Actor)
				{
					UE_LOG(LogTemp, Error, TEXT("ACTOR NOT FOUND"));
					continue;
				}

				PutItemInPool(NewState, ItemData.StaticData.ItemId, Actor);
				return Actor;
			}
		}
		else
		{
			Pool.RemoveAtSwap(i);
			Actor->SetActorHiddenInGame(false);
			Actor->SetActorEnableCollision(true);
			Actor->SetActorTickEnabled(true);

			if (!Actor)
			{
				UE_LOG(LogTemp, Error, TEXT("ACTOR NOT FOUND"));
				continue;
			}
			Actor->SetRuntimeData(ItemData, NewState);
			PutItemInPool(NewState, ItemData.StaticData.ItemId, Actor);
			return Actor;
		}
	}

	// If we get here, no matching actor was found
	UE_LOG(LogTemp, Warning, TEXT("Creating new item for %s"), *ItemData.StaticData.ItemId.ToString());

	FItemData* NewItemData = ItemDataTable->FindRow<FItemData>(ItemData.StaticData.ItemId, "");
	if (!NewItemData)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemData not found in table for ID: %s"), *ItemData.StaticData.ItemId.ToString());
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACPP_PickupActor* NewActor = GetWorld()->SpawnActor<ACPP_PickupActor>(
		PickupActor,
		FVector(0, 0, 0),
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (NewItem)
	{
		NewActor->ResetToPoolState();
		NewActor->SetActorHiddenInGame(true);
		NewActor->SetActorEnableCollision(false);
		NewActor->SetActorTickEnabled(false);
	}

	NewActor->Initialize(*NewItemData);
	NewActor->SetRuntimeData(ItemData, NewState);
	Pool.Add(NewActor);
	PutItemInPool(NewState, ItemData.StaticData.ItemId, NewActor);

	return NewActor;
}

void UCPP_ObjectPoolManager::ReturnPickupActor(const FItemRuntimeInfo& ItemData, EItemState& NewState, const FTransform& PoolPosition)
{
	TArray<ACPP_PickupActor*>& Pool = GetPool(ItemData.ItemState, ItemData.StaticData.ItemId);
	if (Pool.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("couldnt find an actor in assigned pool"));
		return;
	}
	for (size_t i = 0; i < Pool.Num(); i++)
	{

			ACPP_PickupActor* Actor = Pool[i];
			if (!Actor || Actor->IsActorBeingDestroyed())
			{
				UE_LOG(LogTemp, Warning, TEXT("couldnt find an actor in assigned pool"));
				return;
			}
			Pool.Remove(Actor);
			if (ItemData.ItemState != EItemState::Pool) Actor->SetItemState(NewState);
			if (NewState == EItemState::Pool)
			{
				Actor->ResetRuntimeData();
				Actor->SetActorHiddenInGame(true);
				Actor->SetActorEnableCollision(false);
				Actor->SetActorTickEnabled(false);
				Actor->SetActorTransform(PoolPosition);
			}

			PutItemInPool(NewState, ItemData.StaticData.ItemId, Actor);
		}
}

void UCPP_ObjectPoolManager::ReturnEquipActor()
{
	if (!CurrentEquipActor) return;

	CurrentEquipActor->SetActorHiddenInGame(true);
	CurrentEquipActor->SetActorEnableCollision(false);
	CurrentEquipActor->SetActorTickEnabled(false);
	CurrentEquipActor->SetActorLocation(FVector(0, 50, 0));
	AllEquipActors.FindOrAdd(CurrentEquipActor->GetGeneralData().ItemId).Add(CurrentEquipActor);
	CurrentEquipActor = nullptr;
}

void UCPP_ObjectPoolManager::ReturnAllPickupActors()
{
	for (auto& Pair : PickupLevelPool)
	{
		for (ACPP_PickupActor* Actor : Pair.Value)
		{
			if (!Actor || Actor->IsActorBeingDestroyed()) continue;
			Actor->ResetRuntimeData();
			Actor->SetActorHiddenInGame(true);
			Actor->SetActorEnableCollision(false);
			Actor->SetActorTickEnabled(false);
			Actor->SetActorLocation(FVector(0, 50, 0));
			PutItemInPool(EItemState::Pool, Pair.Key, Actor);
		}
	}
	PickupLevelPool.Empty();
}

TArray<ACPP_PickupActor*>& UCPP_ObjectPoolManager::GetPool(EItemState State, FName ItemId)
{
	 if (State == EItemState::Level)
	{
		return PickupLevelPool.FindOrAdd(ItemId);
	}
	 else if (State == EItemState::Inventory)
	 {
		 return AllPickupActors.FindOrAdd(ItemId);
	 }
	 return AllPickupActors.FindOrAdd(ItemId);
}

void UCPP_ObjectPoolManager::PutItemInPool(EItemState State, const FName& ItemId, ACPP_PickupActor* Actor)
{
	switch (State)
	{
	case EItemState::Level:
		PickupLevelPool.FindOrAdd(ItemId).Add(Actor);
		break;
	case EItemState::Inventory:
		AllPickupActors.FindOrAdd(ItemId).Add(Actor);
		break;
	case EItemState::Pool:
		AllPickupActors.FindOrAdd(ItemId).Add(Actor);
		break;
	default:
		break;
	}
}
