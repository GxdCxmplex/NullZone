#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NpcInfo.generated.h"

UENUM(BlueprintType)
enum class ENpcGeneralState : uint8
{
	Passive, 
	Attacking,
	Stunned,
	Investigating,
	Dead
};

UENUM(BlueprintType)
enum class ENpcType : uint8
{
	Meelee,
	Riffle,
	Pistol,
	Shotgun,
	Sniper
};

UENUM(BlueprintType)
enum  class ECombatRange : uint8
{
	Close,
	Mid,
	Far
};

UENUM(BlueprintType)
enum class EMovementSpeed : uint8
{
	Idle,
	Walking,
	Sprinting,
	Jogging
};


USTRUCT(BlueprintType)
struct NULLZONE_API FNpcStaticData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	ENpcType NpcType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float WanderRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Perception")
	float SightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Perception")
	float LoseSightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Perception")
	float PeripheralVisionAngleDegrees;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	TSubclassOf<class UAnimInstance> AnimBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	class UBehaviorTree* NPC_BT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FName WeaponId;

	FNpcStaticData()
		: MaxHealth(100.0f)
		, MaxStamina(100.0f)
		, MovementSpeed(600.0f)
		, AttackRange(150.0f)
		, AttackDamage(20.0f)
		, WanderRadius(500.0f)
		, AnimBP(nullptr)
		, NPC_BT(nullptr)
	{
	}
};


USTRUCT(BlueprintType)
struct NULLZONE_API FNPCRuntimeData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float Stamina;

	UPROPERTY()
	FNpcStaticData StaticData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	ENpcGeneralState GeneralState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	ECombatRange PreferedRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float MinCombatDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float IdealCombatDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float MaxCombatDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float WanderRadius;

	FNPCRuntimeData()
		: Health(100.0f)
		, Stamina(100.0f)
		, StaticData(FNpcStaticData())
	{
	}
};