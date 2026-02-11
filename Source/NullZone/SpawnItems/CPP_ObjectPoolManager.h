// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NullZone/Data/ItemInfo.h"
#include "CPP_ObjectPoolManager.generated.h"

class ACPP_EquipActor;
class ACPP_PickupActor;
class UDataTable;

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NULLZONE_API UCPP_ObjectPoolManager : public UActorComponent
{
	GENERATED_BODY()

public:	

	UCPP_ObjectPoolManager();

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void InitializePools(UDataTable* TempItemDataTable, TSubclassOf<ACPP_EquipActor> EquipClass, TSubclassOf<ACPP_PickupActor> PickupClass);

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	ACPP_EquipActor* GetEquipActor(const FItemRuntimeInfo& ItemInfo);
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	ACPP_EquipActor* GetEquipActorForNpc(const FName& ItemId);

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	ACPP_PickupActor* GetPickupActor(const ACPP_PickupActor* NewItem, const FItemRuntimeInfo& ItemData, EItemState NewState);

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void ReturnPickupActor(const FItemRuntimeInfo& ItemData, EItemState& NewState, const FTransform& PoolPosition);

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void ReturnEquipActor();

	void ReturnAllPickupActors();

	TArray<ACPP_PickupActor*>& GetPool(EItemState State, FName ItemId);
	void PutItemInPool(EItemState State, const FName& ItemId, ACPP_PickupActor* Actor);

private:

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	TMap<FName, int32> ItemsPoolSizes;

	//actors that are currently visible in level
	TMap<FName, TArray<ACPP_PickupActor*>> PickupLevelPool;
	//actors that are in player's inventory

	ACPP_EquipActor* CurrentEquipActor;

	// all actors in pool, unused
	TMap<FName, TArray<ACPP_PickupActor*>> AllPickupActors;
	TMap<FName, TArray<ACPP_EquipActor*>> AllEquipActors;

	
	UPROPERTY()
	UDataTable* ItemDataTable;

	UPROPERTY()
	TSubclassOf<ACPP_PickupActor> PickupActor;

	UPROPERTY()
	TSubclassOf<ACPP_EquipActor> EquipActor;
};
