// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NullZone/Data/ItemInfo.h"
#include "CPP_WaveManager.generated.h"

class ACPP_ItemManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnWaveUpdateInfo, EWavePhase, WavePhase, float, TimeRemaining, int32, WaveNum);


USTRUCT(BlueprintType)
struct FLootSpawnPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FTransform Transform;

	UPROPERTY(VisibleAnywhere)
	bool bOcupied;

	FLootSpawnPoint()
		: Transform(FTransform::Identity)
		, bOcupied(false)
	{}

	FLootSpawnPoint(const FTransform& Trans) 
		: Transform(Trans)
		, bOcupied(false)
	{}
};

UCLASS()
class NULLZONE_API ACPP_WaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPP_WaveManager();

	void StartWave(int32 WaveIndex);

	UPROPERTY(BlueprintAssignable)
	FOnWaveUpdateInfo OnWaveUpdateInfo;

protected:
	virtual void BeginPlay() override;

	void EnterPreparation();
	void EnterWave();
	void EnterCleanup();

	void BuildLootTable();
	const FItemData* PickRandomItem() const;

	void SpawnLoot();
	void CleanupLoot();

	bool ShouldSpawn();

	FItemRuntimeInfo ConvertToRuntime(const FItemData* Data);
	

private:	

	EWavePhase CurrentPhase = EWavePhase::None;

	int32 CurrentWave = 0;

	float WaveDuration = 10.f;
	float TimeRemaining = 0.f;

	float SpawnChance = .65f;
	float TotalWeight = 0.f;

	FTimerHandle WaveTimerHandle;

	int32 AliveEnemies = 0;

	UPROPERTY()
	ACPP_ItemManager* ItemManager;

	UPROPERTY(EditAnywhere)
	TArray<FLootSpawnPoint> LootSpawnPoints;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	UDataTable* ItemDataTable;

	TArray<FItemData*> LootTable;

};
