// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_WaveManager.h"
#include "CPP_ItemManager.h"
#include "CPP_LootSpawnPoint.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "EngineUtils.h"



ACPP_WaveManager::ACPP_WaveManager()
{
	PrimaryActorTick.bCanEverTick = false;
}



void ACPP_WaveManager::BeginPlay()
{
	Super::BeginPlay();

	ItemManager = Cast<ACPP_ItemManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACPP_ItemManager::StaticClass()));
	
	for (TActorIterator<ACPP_LootSpawnPoint> It(GetWorld()); It; ++It)
	{
		LootSpawnPoints.Add(FLootSpawnPoint(It->GetActorTransform()));
	}
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Item Data Table not assigned in Wave Manager"))
		return;
	}
	ItemManager->InitializePool();
	TimeRemaining = 5.f;
	CurrentPhase = EWavePhase::Cleanup;
	OnWaveUpdateInfo.Broadcast(CurrentPhase, TimeRemaining, CurrentWave);
	GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &ACPP_WaveManager::EnterWave, 1.f, false);

}

void ACPP_WaveManager::StartWave(int32 WaveIndex)
{
	CurrentPhase = EWavePhase::Preparation;
	TimeRemaining = WaveDuration;
	OnWaveUpdateInfo.Broadcast(CurrentPhase, TimeRemaining, CurrentWave);
	GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &ACPP_WaveManager::EnterWave, 1.f, false);
}

void ACPP_WaveManager::EnterPreparation()
{
	CurrentPhase = EWavePhase::Preparation;
	CurrentWave++;
	BuildLootTable();
	SpawnLoot();
	StartWave(CurrentWave);
}

void ACPP_WaveManager::EnterWave()
{
	TimeRemaining--;
	if (TimeRemaining <= 0.f)
	{
		switch (CurrentPhase)
		{
		case EWavePhase::Cleanup:
		{
			EnterPreparation();
			break;
		}
		case EWavePhase::Preparation:
		{
			EnterCleanup();
			break;
		}
		default:
		{
			break;
		}
			
		}
	}
	else
	{
		if (CurrentPhase == EWavePhase::Cleanup)
		{
			OnWaveUpdateInfo.Broadcast(CurrentPhase, TimeRemaining, CurrentWave);
			GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &ACPP_WaveManager::EnterWave, 1.f, false);
		}
		else if (CurrentPhase == EWavePhase::Preparation)
		{
			OnWaveUpdateInfo.Broadcast(CurrentPhase, TimeRemaining, CurrentWave);
			GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &ACPP_WaveManager::EnterWave, 1.f, false);
		}

	}
}

void ACPP_WaveManager::EnterCleanup()
{
	CurrentPhase = EWavePhase::Cleanup;
	TimeRemaining = 3.f;
	OnWaveUpdateInfo.Broadcast(CurrentPhase, TimeRemaining, CurrentWave);
	CleanupLoot();
	GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &ACPP_WaveManager::EnterWave, 1.f, false);
}

void ACPP_WaveManager::BuildLootTable()
{
	LootTable.Empty();
	TotalWeight = 0.f;

	TArray<FName> RowNames = ItemDataTable->GetRowNames();
	for (const FName& ItemID : RowNames)
	{
		FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemID, "");
		LootTable.Add(ItemData);
		TotalWeight += ItemData->SpawnWeight;
	}
}

const FItemData* ACPP_WaveManager::PickRandomItem() const
{
	float Roll = FMath::FRandRange(.0f, TotalWeight);

	float Accumulated = 0.f;
	for (const FItemData* Row : LootTable)
	{
		Accumulated += Row->SpawnWeight;
		if (Roll <= Accumulated)
		{
			return Row;
		}
	}
	return nullptr;
}

void ACPP_WaveManager::SpawnLoot()
{
	if (!ItemManager) return;
	for (FLootSpawnPoint& SpawnPoint : LootSpawnPoints)
	{
		if (SpawnPoint.bOcupied) continue;
		if (!ShouldSpawn()) continue;
		const FItemData* Row = PickRandomItem();
		if (!Row) continue;
		ItemManager->SpawnItemInWorld(nullptr, ConvertToRuntime(Row), EItemState::Level, SpawnPoint.Transform);
		SpawnPoint.bOcupied = true;
	}
}

void ACPP_WaveManager::CleanupLoot()
{
	ItemManager->CleanupMap();
	for (FLootSpawnPoint& SpawnPoint : LootSpawnPoints)
	{
		SpawnPoint.bOcupied = false;
	}
}

bool ACPP_WaveManager::ShouldSpawn()
{
	return FMath::FRand() <= SpawnChance;
}

FItemRuntimeInfo ACPP_WaveManager::ConvertToRuntime(const FItemData* Data)
{
	FItemRuntimeInfo IRI;
	IRI.AmmoData = Data->AmmoData;
	IRI.ItemQuantity = Data->ItemQuantity;
	IRI.ItemState = EItemState::Pool;
	IRI.ItemWeight = Data->ItemWeight;
	IRI.MedData = Data->MedData;
	IRI.MeeleeData = Data->MeeleeData;
	IRI.StaticData = *Data;
	IRI.WeaponData = Data->WeaponData;
	IRI.InstanceId = FGuid::NewGuid();

	return IRI;
}



