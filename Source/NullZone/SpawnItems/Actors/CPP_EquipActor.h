// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NullZone/Data/ItemInfo.h"
#include "NullZone/Equippable.h"
#include "CPP_EquipActor.generated.h"

class ACPP_PlayerCharacter;

enum class EMeeleeAttackType : uint8
{
	Light,
	Heavy
};

UCLASS()
class NULLZONE_API ACPP_EquipActor : public AActor, public IEquippable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_EquipActor();

	/* interfaces */
	virtual void StartUseM1_Implementation(ACPP_PlayerCharacter* Character) override;
	virtual void StartUseM2_Implementation(ACPP_PlayerCharacter* Character) override;
	virtual void StopUseM1_Implementation() override;
	virtual void StopUseM2_Implementation() override;
	float GetUseProgress() const;

	void UseMeds();
	void UseWeapon();
	void UseMeeleeAttack(EMeeleeAttackType Type);
	void PerformMeleeTrace(EMeeleeAttackType Type);
	void UseLightMeeleeOnM1Hold();
	void ExecuteHeavyAttack();
	void ResetCombo();
	void ResetAttackCooldown();

	void ChangedItem();

	void Reloading(ACPP_PlayerCharacter* Character);
	void Unloading(ACPP_PlayerCharacter* Character);

	void Initialize(const FItemData& InItemData);
	void SetRuntimeData(const FItemRuntimeInfo& RuntimeData);
	void ResetRuntimeData();
	void ResetToPoolState();
	void FinishUse();

	FORCEINLINE FItemRuntimeInfo GetRuntimeInfo() { return RuntimeData; }
	FORCEINLINE FItemData GetGeneralData()  { return GeneralData; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemData GeneralData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemRuntimeInfo RuntimeData;

	FTimerHandle UseTimer;
	FTimerHandle M2UseTimer;
	FTimerHandle ComboResetTimer;
	FTimerHandle HeaveChargeTimer;
	FTimerHandle AttackCooldownTimer;
	
	bool bIsUsing = false;
	bool m2IsUsing = false;
	float UseElapsed = .0f;

	ACPP_PlayerCharacter* OwningCharacter = nullptr;

	void TickUse();

	FTransform OgTransform;
};
