// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NullZone/Data/ItemInfo.h"
#include "NullZone/Equippable.h"
#include "NullZone/UserInterface.h"
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
	virtual void StartUseM1_Implementation(AActor* User, bool bIsPlayer) override;
	virtual void StartUseM2_Implementation(AActor* User, bool bIsPlayer) override;
	virtual void StopUseM1_Implementation() override;
	virtual void StopUseM2_Implementation() override;
	float GetUseProgress() const;

	void UseMeds();
	void UseWeapon();
	void BeginMeleeHit(float InDamage, AActor* InUser);
	void EndMeleeHit();

	UFUNCTION()
	void OnHitOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ChangedItem();

	void Reloading(AActor* User, bool bIsPlayer);
	void Unloading(AActor* User, bool bIsPlayer);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* HitCollider;

	FTimerHandle UseTimer;
	FTimerHandle M2UseTimer;
	
	bool bIsUsing = false;
	bool m2IsUsing = false;
	float UseElapsed = .0f;

	TScriptInterface<IUserInterface> OwningCharacter;
	AActor* OwningActor = nullptr;
	bool IsPlayer = false;

	void TickUse();

	TSet<AActor*> HitActors;

	FTransform OgTransform;

	float Damage = 0;
};
