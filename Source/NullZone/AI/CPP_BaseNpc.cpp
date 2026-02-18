// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseNpc.h"
#include "Components/SphereComponent.h"

#include "CPP_BaseNPCController.h"
#include "NullZone/SpawnItems/CPP_ItemManager.h"
#include "NullZone/SpawnItems/Actors/CPP_EquipActor.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

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
	if (DamageCauser == this) return;
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
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ACPP_BaseNpc::attachWeapon, 1, false);
}

void ACPP_BaseNpc::SetAttackToTrue()
{
	if (!BB) return;
	BB->SetValueAsBool("CanAttack", true);
}

void ACPP_BaseNpc::ResetAttackCooldown()
{
	SetAttackBool(true);
}

void ACPP_BaseNpc::ResetCombo()
{
	ComboIndex = 0;
}

bool ACPP_BaseNpc::AttackFunc()
{
	if (!CanAttack() || !EquippedItem) return false;
	AActor* CurrentTarget = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
	if (!CurrentTarget) return false;
	StartLightAttack();
	return true;
	/*SetAttackBool(false);
	//BB->SetValueAsBool("CanAttack", false);
	if (StaticData.NpcType == ENpcType::Meelee)
	{
		if (FMath::FRand() >= .0f)
		{
			GetWorldTimerManager().SetTimer(AttackTimer, this, &ACPP_BaseNpc::SetAttackToTrue, EquippedItem->GetRuntimeInfo().MeeleeData.LightCooldown, false);
			if (!LightComboMonatge) return false;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && !AnimInstance->Montage_IsPlaying(LightComboMonatge))
			{
				AnimInstance->Montage_Play(LightComboMonatge);
				switch (EquippedItem->GetRuntimeInfo().MeeleeData.CurrentComboIndex)	
				{
				case 0:
					AnimInstance->Montage_JumpToSection(FName("Section_1"), LightComboMonatge);
					UE_LOG(LogTemp, Warning, TEXT("c1"));
					break;
				case 1:
					AnimInstance->Montage_JumpToSection(FName("Section_2"), LightComboMonatge);
					UE_LOG(LogTemp, Warning, TEXT("c2"));
					break;
				case 2:
					AnimInstance->Montage_JumpToSection(FName("Section_3"), LightComboMonatge);
					UE_LOG(LogTemp, Warning, TEXT("c3"));
					break;
				default:
					break;
				}
				
			}
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
	return true;*/
}

void ACPP_BaseNpc::StartLightAttack()
{
	if (!LightComboMonatge) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	SetAttackBool(false);
	SetIsAttacking(true);

	AnimInstance->Montage_Play(LightComboMonatge);
	const FName SectionName = FName(*FString::Printf(TEXT("Section_%d"), ComboIndex + 1));
	AnimInstance->Montage_JumpToSection(SectionName, LightComboMonatge);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &ACPP_BaseNpc::ResetAttackCooldown, EquippedItem->GetRuntimeInfo().MeeleeData.LightCooldown, false);
	GetWorldTimerManager().SetTimer(ComboResetTimer, this, &ACPP_BaseNpc::ResetCombo, EquippedItem->GetRuntimeInfo().MeeleeData.ComboResetTime, false);
}

void ACPP_BaseNpc::LightAttackBegin()
{
	if (EquippedItem)
	{
		float Damage;
		switch (ComboIndex)
		{
		case 0:
			Damage = EquippedItem->GetRuntimeInfo().MeeleeData.LightDamage1;
			break;
		case 1:
			Damage = EquippedItem->GetRuntimeInfo().MeeleeData.LightDamage2;
			break;
		case 2:
			Damage = EquippedItem->GetRuntimeInfo().MeeleeData.LightDamage3;
			break;
		default:
			break;

		}
		ComboIndex++;
		if (ComboIndex == MaxComboCount) ComboIndex = 0;
		UE_LOG(LogTemp, Warning, TEXT("Damage: %f"), Damage);
		EquippedItem->BeginMeleeHit(Damage, this);
	}
}

void ACPP_BaseNpc::OnAttackEnd()
{
	if (EquippedItem)
	{
		EquippedItem->EndMeleeHit();
	}

	SetIsAttacking(false);
}

void ACPP_BaseNpc::attachWeapon()
{
	EquippedItem = ItemManager->EquipItemToNpc(StaticData.WeaponId, this);
}

bool ACPP_BaseNpc::CanAttack() const
{
	if (!BB) return false;
	return BB->GetValueAsBool("CanAttack");
}

void ACPP_BaseNpc::SetIsAttacking(bool bValue)
{
	if (!BB) return;
	BB->SetValueAsBool("IsAttacking", bValue);
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


