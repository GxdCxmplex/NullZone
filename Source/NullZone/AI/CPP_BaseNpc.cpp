// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseNpc.h"
#include "Components/SphereComponent.h"

#include "CPP_BaseNPCController.h"
#include "NullZone/SpawnItems/CPP_ItemManager.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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
		NewMaxSpeed = 200.f;
		break;
	case EMovementSpeed::Sprinting:
		NewMaxSpeed = 650.f;
		break;
	case EMovementSpeed::Jogging:
		NewMaxSpeed = 400.f;
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

void ACPP_BaseNpc::AttackFunc()
{
	AActor* CurrentTarget = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
	if (!CurrentTarget) return;
	if (CurrentTarget->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_TakeDamage(CurrentTarget, StaticData.AttackDamage, this);
	}
}

void ACPP_BaseNpc::LoadNpcData()
{
	if (!NPCDataTable) return;

	FNpcStaticData* Row = NPCDataTable->FindRow<FNpcStaticData>(FName(Name), TEXT(""));

	if (!Row) return;

	StaticData = *Row;

	RuntimeData.Health = StaticData.MaxHealth;
	RuntimeData.Stamina = StaticData.MaxStamina;
	RuntimeData.StaticData = StaticData;

	GetCharacterMovement()->MaxWalkSpeed = StaticData.MovementSpeed;

	BB->SetValueAsFloat("WanderRadius", StaticData.WanderRadius);
	
	AI->InitializePerceptionFromNPC(StaticData);

	UE_LOG(LogTemp, Warning, TEXT("Data Assigned"));
}


