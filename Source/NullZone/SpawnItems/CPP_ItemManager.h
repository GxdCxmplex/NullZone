// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NullZone/Data/ItemInfo.h"
#include "CPP_ItemManager.generated.h"

class ACPP_PickupActor;
class ACPP_EquipActor;
class UCPP_ObjectPoolManager;

UCLASS()
class NULLZONE_API ACPP_ItemManager : public AActor
{
	GENERATED_BODY()
	
public:

	ACPP_ItemManager();

	UFUNCTION(BlueprintCallable)
	void InitializePool();

	UFUNCTION(BlueprintCallable)
	ACPP_PickupActor* SpawnItemInWorld(const ACPP_PickupActor* NewItem, const FItemRuntimeInfo& ItemData, EItemState NewState, const FTransform& SpawnTransform);

	UFUNCTION(BlueprintCallable)
	ACPP_EquipActor* EquipItemToPlayer(const FItemRuntimeInfo& ItemData, const ACharacter* Player);

	UFUNCTION(BlueprintCallable)
	ACPP_EquipActor* EquipItemToNpc(const FName& ItemId, const ACharacter* Npc);

	UFUNCTION(BlueprintCallable)
	void DropItemFromPlayer(FItemRuntimeInfo& ItemData, EItemState NewState, const FTransform& DropTransform);

	void CleanupMap();

	FORCEINLINE FTransform GetDefaultPoolPlace() const { return DefaultPoolPlace; }

protected:
	UPROPERTY(EditAnywhere, Category = "Object Pool")
	TSubclassOf<ACPP_EquipActor> EquipActorClass;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	TSubclassOf<ACPP_PickupActor> PickupActorClass;

private:

	UPROPERTY()
	FTransform DefaultPoolPlace;

	UPROPERTY()
	UCPP_ObjectPoolManager* PoolManager;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	UDataTable* ItemDataTable;


};
