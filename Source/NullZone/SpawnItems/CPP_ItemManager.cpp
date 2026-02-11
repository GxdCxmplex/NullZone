// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ItemManager.h"
#include "Actors/CPP_EquipActor.h"
#include "Actors/CPP_PickupActor.h"
#include "GameFramework/Character.h"
#include "CPP_ObjectPoolManager.h"

// Sets default values
ACPP_ItemManager::ACPP_ItemManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PoolManager = CreateDefaultSubobject<UCPP_ObjectPoolManager>(TEXT("PoolManager"));
	
	FTransform Transform(FRotator(0, 0, 0), FVector(500, 500, 100), FVector(1, 1, 1));

	DefaultPoolPlace = Transform;
}

void ACPP_ItemManager::InitializePool()
{
	if (!ItemDataTable || !PickupActorClass) return;
	PoolManager->InitializePools(ItemDataTable, EquipActorClass, PickupActorClass);
}


ACPP_PickupActor* ACPP_ItemManager::SpawnItemInWorld(const ACPP_PickupActor* NewItem, const FItemRuntimeInfo& ItemData, EItemState NewState, const FTransform& SpawnTransform)
{
	if (!PoolManager) return nullptr;
	
	ACPP_PickupActor* Actor = PoolManager->GetPickupActor(NewItem, ItemData, NewState);
	if (!Actor)
	{
		UE_LOG(LogTemp, Error, TEXT("ACTOR NOT FOUND"))
			return nullptr;
	}
	Actor->SetActorTransform(SpawnTransform);
	return Actor;
}

ACPP_EquipActor* ACPP_ItemManager::EquipItemToPlayer(const FItemRuntimeInfo& ItemData, const ACharacter* Player)
{
	if (!PoolManager || !Player) return nullptr;

	ACPP_EquipActor* Actor = PoolManager->GetEquipActor(ItemData);
	if (!Actor) return nullptr;
	//Actor->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "");
	return Actor;
}

ACPP_EquipActor* ACPP_ItemManager::EquipItemToNpc(const FName& ItemId, const ACharacter* Npc)
{
	if (!PoolManager || !Npc) return nullptr;
	ACPP_EquipActor* Actor = PoolManager->GetEquipActorForNpc(ItemId);
	if (!Actor) return nullptr;
	return Actor;
}

void ACPP_ItemManager::DropItemFromPlayer(FItemRuntimeInfo& ItemData, EItemState NewState, const FTransform& DropTransform)
{
	if (!PoolManager) return;

	if (ItemData.ItemState == EItemState::Equip)
	{
		PoolManager->ReturnEquipActor();
	}
	SpawnItemInWorld(nullptr, ItemData, NewState, DropTransform);
}

void ACPP_ItemManager::CleanupMap()
{
	PoolManager->ReturnAllPickupActors();
}