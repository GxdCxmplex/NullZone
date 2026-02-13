#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemInfo.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None        UMETA(DisplayName = "None"),
	Weapon      UMETA(DisplayName = "Weapon"),
	Meelee      UMETA(DisplayName = "Meelee"),
	Armor       UMETA(DisplayName = "Armor"),
	Meds		UMETA(DisplayName = "Meds"),
	Ammo        UMETA(DisplayName = "Ammo"),
	Utility     UMETA(DisplayName = "Utility"),

};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Melee,
	Pistol,
	AR,
	Shotgun,
	Sniper,
};

UENUM(BlueprintType)
enum class EMedsType : uint8
{
	FirstAidKit,
	AidKit,
	Bandage,
	Painkiller,
	SpeedBoost,
	DmgBoost,
	Tourniquet,
};

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	CompactAmmo,
	MachinePistol,
	ServiceAmmo,
	IntermediateHV,
	PrecisionAmmo,
	LongRangeAmmo,
	ShotgunShell,
	HighCaliberSurplus
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	Level,
	Inventory,
	Pool,
	Equip
};

UENUM(BlueprintType)
enum class EWavePhase : uint8
{
	None,
	Preparation,
	Wave,
	Cleanup
};

UENUM(BlueprintType)
enum class EWeaponFireType : uint8
{
	Semi,
	Auto
};


USTRUCT(BlueprintType)
struct NULLZONE_API FWeaponItemData
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MagSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 Ammos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Spread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponFireType FireType;


	FWeaponItemData() = default;

	FWeaponItemData(EAmmoType InAmmoType, int32 InMagSize, float InFireRate, float InReloadTime, int32 InBaseDamage)
		: WeaponType(EWeaponType::AR),
		AmmoType(InAmmoType)
		, MagSize(InMagSize)
		, FireRate(InFireRate)
		, ReloadTime(InReloadTime)
		, BaseDamage(InBaseDamage)
	{}
};

USTRUCT(BlueprintType)
struct NULLZONE_API FMeeleeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	int32 CurrentComboIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float ComboResetTime;

	// Light
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float LightDamage1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float LightDamage2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float LightDamage3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float LightRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float LightCooldown;

	// Heavy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float HeavyDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float HeavyRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float HeavyCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float HeavyChargeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float TraceRadius;

	FMeeleeData()
		: CurrentComboIndex(0)
		, ComboResetTime(1.0f)
		, LightDamage1(15.f)
		, LightDamage2(20.f)
		, LightDamage3(25.f)
		, LightRange(200.f)
		, LightCooldown(0.3f)
		, HeavyDamage(60.f)
		, HeavyRange(250.f)
		, HeavyCooldown(1.2f)
		, HeavyChargeTime(0.6f)
		, TraceRadius(80.f)
	{
	}
};

USTRUCT(BlueprintType)
struct NULLZONE_API FMedData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMedsType MedType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ApplyTime;

	FMedData() = default;

	FMedData(EMedsType InMedType, float InHealAmount)
		: MedType(InMedType)
		, HealAmount(InHealAmount)
	{
	}
};

USTRUCT(BlueprintType)
struct NULLZONE_API FAmmoData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;

};


USTRUCT(BlueprintType)
struct NULLZONE_API FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsStackable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemQuantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ItemWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float SpawnWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FWeaponItemData WeaponData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FMedData MedData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FMeeleeData MeeleeData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FAmmoData AmmoData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	UStaticMesh* PickupMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	USkeletalMesh* EquippedMesh = nullptr;


	FItemData()
		: ItemType(EItemType::None)
		, ItemId(FName(""))
		, ItemDescription(TEXT(""))
		, ItemName(TEXT("Empty"))
		, bIsStackable(false)
		, ItemQuantity(0)
	{
	}
};

USTRUCT(BlueprintType)
struct NULLZONE_API FItemRuntimeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemData StaticData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemState ItemState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemQuantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ItemWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FWeaponItemData WeaponData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FMedData MedData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FMeeleeData MeeleeData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FAmmoData AmmoData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FGuid InstanceId;

	FItemRuntimeInfo()
		: StaticData(FItemData())
		, ItemState(EItemState::Pool)
		, ItemQuantity(1)
		, ItemWeight(1.f)
		
	{ }

	FItemRuntimeInfo(FItemData StatD)
		: StaticData(StatD)
		, ItemState(EItemState::Pool)
		, ItemQuantity(0)
		, ItemWeight(1.f)

	{
	}
};