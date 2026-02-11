// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NullZone/CombatInterface.h"
#include "SetMovementSpeed.h"
#include "NullZone/Data/NpcInfo.h"
#include "CPP_BaseNpc.generated.h"

class USphereComponent;
class ACPP_BaseNPCController;
class UDataTable;
class ACPP_EquipActor;

UCLASS()
class NULLZONE_API ACPP_BaseNpc : public ACharacter, public ICombatInterface, public ISetMovementSpeed
{
	GENERATED_BODY()

public:
	ACPP_BaseNpc();

	virtual void TakeDamage_Implementation(float Damage, AActor* DamageCauser) override;
	virtual void SetMovementSpeed_Implementation(EMovementSpeed NewSpeed) override;

	void AttackFunc();

	void LoadNpcData();

	FORCEINLINE FNpcStaticData& GetStaticData() { return StaticData; }

	FORCEINLINE FNPCRuntimeData GetRuntimeData() { return RuntimeData; }

	FORCEINLINE ACPP_EquipActor* GetEquippedItem() { return EquippedItem; }
	FORCEINLINE void SetEquippedItem(ACPP_EquipActor* NewItem) { EquippedItem = NewItem; }

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<ACPP_BaseNPCController> AIControllerBP;

	UPROPERTY()
	FNpcStaticData StaticData;

	UPROPERTY()
	FNPCRuntimeData RuntimeData;

	UPROPERTY(EditDefaultsOnly, Category = "NPC")
	UDataTable* NPCDataTable;

	FName Name = "Test";

	class ACPP_BaseNPCController* AI = nullptr;

	class UBlackboardComponent* BB = nullptr;

	UPROPERTY()
	ACPP_EquipActor* EquippedItem;

	UPROPERTY()
	class ACPP_ItemManager* ItemManager;
};
