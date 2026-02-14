// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseNpc.h"
#include "Components/SphereComponent.h"

#include "CPP_BaseNPCController.h"
#include "NullZone/SpawnItems/CPP_ItemManager.h"
#include "NullZone/SpawnItems/Actors/CPP_EquipActor.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

#include "NullZone/Equippable.h"

#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
ACPP_BaseNpc::ACPP_BaseNpc()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACPP_BaseNpc::TakeDamage_Implementation(float Damage, AActor* DamageCauser)
{
	RuntimeData.Health -= Damage;
	if (!BB) return;
	BB->SetValueAsBool("WasDamagedRecently", true);
	BB->SetValueAsObject("TargetActor", DamageCauser);
	BB->SetValueAsBool("HasTarget", true);
	BB->SetValueAsVector("PointOfinterest", DamageCauser->GetActorLocation());
}

void ACPP_BaseNpc::SetMovementSpeed_Implementation(EMovementSpeed NewSpeed)
{
	float NewMaxSpeed = 0.f;

	switch (NewSpeed)
	{
	case EMovementSpeed::Idle:
		NewMaxSpeed = 0.f;
		break;
	case EMovementSpeed::Walking:
		NewMaxSpeed = 100.f;
		break;
	case EMovementSpeed::Sprinting:
		NewMaxSpeed = 500.f;
		break;
	case EMovementSpeed::Jogging:
		NewMaxSpeed = 350.f;
		break;
	default:
		break;
	}

	GetCharacterMovement()->MaxWalkSpeed = NewMaxSpeed;
}

// Called when the game starts or when spawned
void ACPP_BaseNpc::BeginPlay()
{
	Super::BeginPlay();
	
	if (AIControllerBP)
	{
		AIControllerClass = AIControllerBP;
	}

	AI = Cast<ACPP_BaseNPCController>(GetController());
	if (!AI) return;
	BB = AI->GetBlackboardComponent();
	if (!BB) return;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	LoadNpcData();

	ItemManager = Cast<ACPP_ItemManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACPP_ItemManager::StaticClass()));
	EquippedItem = ItemManager->EquipItemToNpc(StaticData.WeaponId, this);
}

void ACPP_BaseNpc::SetAttackToTrue()
{
	if (!BB) return;
	BB->SetValueAsBool("CanAttack", true);
}

bool ACPP_BaseNpc::AttackFunc()
{
	if (!CanAttack() || !EquippedItem) return false;
	AActor* CurrentTarget = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
	if (!CurrentTarget) return false;
	//BB->SetValueAsBool("CanAttack", false);
	if (StaticData.NpcType == ENpcType::Meelee)
	{
		if (FMath::FRand() >= .7f)
		{
			GetWorldTimerManager().SetTimer(AttackTimer, this, &ACPP_BaseNpc::SetAttackToTrue, EquippedItem->GetRuntimeInfo().MeeleeData.LightCooldown, false);
			IEquippable::Execute_StartUseM1(Cast<UObject>(EquippedItem), this, false);
		}
		else
		{
			GetWorldTimerManager().SetTimer(AttackTimer, this, &ACPP_BaseNpc::SetAttackToTrue, EquippedItem->GetRuntimeInfo().MeeleeData.HeavyCooldown, false);
			IEquippable::Execute_StartUseM2(Cast<UObject>(EquippedItem), this, false);
		}
	}
	else
	{
		GetWorldTimerManager().SetTimer(AttackTimer, this, &ACPP_BaseNpc::SetAttackToTrue, EquippedItem->GetRuntimeInfo().WeaponData.FireRate, false);
		IEquippable::Execute_StartUseM1(Cast<UObject>(EquippedItem), this, false);
	}
	return true;
}

bool ACPP_BaseNpc::CanAttack() const
{
	if (!BB) return false;
	return BB->GetValueAsBool("CanAttack");
}

void ACPP_BaseNpc::SetAttackBool(bool NewBool)
{
	if (!BB) return;
	BB->SetValueAsBool("CanAttack", NewBool);
}

void ACPP_BaseNpc::SetHealth(float NewValue, float MaxValue)
{
	RuntimeData.Health = NewValue;
}

void ACPP_BaseNpc::LoadNpcData()
{
	if (!NPCDataTable) return;

	FNpcStaticData* Row = NPCDataTable->FindRow<FNpcStaticData>(FName(Name), TEXT(""));

	if (!Row) return;

	StaticData = *Row;

	RuntimeData.Health = StaticData.MaxHealth;
	RuntimeData.Stamina = StaticData.MaxStamina;
	RuntimeData.MinCombatDistance = StaticData.MinCombatDistance;
	RuntimeData.IdealCombatDistance = StaticData.IdealCombatDistance;
	RuntimeData.MaxCombatDistance = StaticData.MaxCombatDistance;
	RuntimeData.StaticData = StaticData;


	GetCharacterMovement()->MaxWalkSpeed = StaticData.MovementSpeed;

	BB->SetValueAsFloat("WanderRadius", StaticData.WanderRadius);
	BB->SetValueAsFloat("MinCombatDistance", RuntimeData.MinCombatDistance);
	BB->SetValueAsFloat("IdealCombatDistance", RuntimeData.IdealCombatDistance);
	BB->SetValueAsFloat("MaxCombatDistance", RuntimeData.MaxCombatDistance);
	AI->InitializePerceptionFromNPC(StaticData);

	UE_LOG(LogTemp, Warning, TEXT("Data Assigned"));
}


