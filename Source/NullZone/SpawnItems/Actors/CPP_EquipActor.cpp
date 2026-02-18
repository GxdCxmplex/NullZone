// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_EquipActor.h"
#include "TimerManager.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "NullZone/Player/CPP_PlayerCharacter.h"
#include "NullZone/Player/CPP_PlayerController.h"
#include "NullZone/CombatInterface.h"



// Sets default values
ACPP_EquipActor::ACPP_EquipActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;

	HitCollider = CreateDefaultSubobject<UCapsuleComponent>("HitCollider");
	HitCollider->SetupAttachment(ItemMesh);
	HitCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitCollider->SetCollisionObjectType(ECC_WorldDynamic);
	HitCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMesh->SetCollisionProfileName(TEXT("PhysicActor"));
	ItemMesh->SetSimulatePhysics(false);
	ItemMesh->SetGenerateOverlapEvents(true);
}

void ACPP_EquipActor::StartUseM1_Implementation(AActor* User, bool bIsPlayer)
{
	if (bIsUsing|| !User) return;
	if (User->Implements<UUserInterface>())
	{
		OwningActor = User;
		IsPlayer = bIsPlayer;
		OwningCharacter = TScriptInterface<IUserInterface>(User);
	}
	if (!OwningCharacter) return;
	bIsUsing = true;
	if (RuntimeData.StaticData.ItemType == EItemType::Meds)
	{
		UseMeds();
	}
	else if (RuntimeData.StaticData.ItemType == EItemType::Weapon)
	{
		UseWeapon();
	}
	else if (RuntimeData.StaticData.ItemType == EItemType::Meelee)
	{
		
	}
}

void ACPP_EquipActor::StartUseM2_Implementation(AActor* User, bool bIsPlayer)
{
	if (m2IsUsing || !User) return;
	if (User->Implements<UUserInterface>())
	{
		OwningActor = User;
		IsPlayer = bIsPlayer;
		OwningCharacter = TScriptInterface<IUserInterface>(User);
	}
	if (!OwningCharacter) return;
	m2IsUsing = true;
	if (RuntimeData.StaticData.ItemType == EItemType::Meelee)
	{
		//UseMeeleeAttack(EMeeleeAttackType::Heavy);
	}
}

void ACPP_EquipActor::StopUseM1_Implementation()
{
	if (!bIsUsing || !OwningCharacter) return;
	bIsUsing = false;
	GetWorld()->GetTimerManager().ClearTimer(UseTimer);
	UseElapsed = 0.f;
	if (IsPlayer) OwningCharacter->UpdateProgressBar(GetUseProgress(), RuntimeData.StaticData.ItemName);
	UE_LOG(LogTemp, Log, TEXT("Use cancelled"));
}

void ACPP_EquipActor::StopUseM2_Implementation()
{
	if (!m2IsUsing || !OwningCharacter) return;
	m2IsUsing = false;
}

void ACPP_EquipActor::FinishUse()
{
	if (!bIsUsing || !OwningCharacter) return;
	bIsUsing = false;
	GetWorld()->GetTimerManager().ClearTimer(UseTimer);
	if (RuntimeData.StaticData.ItemType == EItemType::Meds)
	{
		OwningCharacter->SetHealth(FMath::Min(OwningCharacter->GetHealth()+RuntimeData.MedData.HealAmount, OwningCharacter->GetHealthMax()), OwningCharacter->GetHealthMax());
		OwningCharacter->MedApplied(RuntimeData);
		OwningCharacter->UpdateProgressBar(GetUseProgress(), RuntimeData.StaticData.ItemName);
	}
}

float ACPP_EquipActor::GetUseProgress() const
{
	if (!bIsUsing || !OwningCharacter) return 0.0f;
	
	return UseElapsed / RuntimeData.MedData.ApplyTime;
}

void ACPP_EquipActor::UseMeds()
{
	UseElapsed = .0f;
	GetWorld()->GetTimerManager().SetTimer(UseTimer, this, &ACPP_EquipActor::TickUse, 0.05f, true);

}

void ACPP_EquipActor::UseWeapon()
{
	FWeaponItemData WeaponData = RuntimeData.WeaponData;
	if (WeaponData.Ammos == 0) return;
	if (WeaponData.FireType == EWeaponFireType::Auto)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Shot auto"));
		OwningCharacter->GetInventoryComponent()->FireWeapon();
		GetWorld()->GetTimerManager().SetTimer(UseTimer, this, &ACPP_EquipActor::UseWeapon, WeaponData.FireRate, true);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Shot semi"));
		OwningCharacter->GetInventoryComponent()->FireWeapon();
	}
	
	FVector Start;
	FRotator Rot;

	if (IsPlayer) OwningCharacter->GetPlayerController()->GetPlayerViewPoint(Start, Rot);

	FVector ShootDir = FMath::VRandCone(Rot.Vector(), FMath::DegreesToRadians(WeaponData.Spread));

	FVector End = Start + (ShootDir * WeaponData.Range);
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(OwningActor);
	Params.bReturnPhysicalMaterial = true;

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel1, Params);
	FColor Color = bHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), Start, bHit ? Hit.ImpactPoint : End, Color, false, 1.f, 0, 1.5f);
	if (bHit)
	{
		DrawDebugSphere(
			GetWorld(),
			Hit.ImpactPoint,
			5.f,
			12,
			FColor::Red,
			false,
			1.f
		);
	}
}

void ACPP_EquipActor::BeginMeleeHit(float InDamage, AActor* InUser)
{
	if (!InUser) return;

	OwningActor = InUser;
	Damage = InDamage;

	HitActors.Empty();
	HitCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACPP_EquipActor::EndMeleeHit()
{
	HitCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitActors.Empty();
}

void ACPP_EquipActor::OnHitOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == OwningActor || !OtherActor->Implements<UCombatInterface>() || HitActors.Contains(OtherActor)) return;
	HitActors.Add(OtherActor);
	ICombatInterface::Execute_TakeDamage(Cast<UObject>(OtherActor), Damage, OwningActor);
}

void ACPP_EquipActor::ChangedItem()
{
	GetWorld()->GetTimerManager().ClearTimer(UseTimer);
	UseElapsed = 0.f;
	if (OwningCharacter) OwningCharacter->UpdateProgressBar(GetUseProgress(), RuntimeData.StaticData.ItemName);
	OwningCharacter = nullptr;
}

void ACPP_EquipActor::Reloading(AActor* User, bool bIsPlayer)
{
	if (!User) return;
	if (User->Implements<UUserInterface>())
	{
		OwningActor = User;
		IsPlayer = bIsPlayer;
		OwningCharacter = TScriptInterface<IUserInterface>(User);
	}
	if (!OwningCharacter) return;
	if (OwningCharacter->GetInventoryComponent()->ReloadWeapon())
	{
		//reload animation
	}
}

void ACPP_EquipActor::Unloading(AActor* User, bool bIsPlayer)
{
	if (!User) return;
	if (User->Implements<UUserInterface>())
	{
		OwningActor = User;
		IsPlayer = bIsPlayer;
		OwningCharacter = TScriptInterface<IUserInterface>(User);
	}
	if (!OwningCharacter) return;
	if (OwningCharacter->GetInventoryComponent()->UnloadWeapon())
	{
		//unload animation
	}
}

// Called when the game starts or when spawned
void ACPP_EquipActor::BeginPlay()
{
	Super::BeginPlay();

	HitCollider->OnComponentBeginOverlap.AddDynamic(this, &ACPP_EquipActor::OnHitOverlap);
}

void ACPP_EquipActor::TickUse()
{
	if (!bIsUsing || !OwningCharacter) return;
	UseElapsed += 0.05f;
	OwningCharacter->UpdateProgressBar(GetUseProgress(), RuntimeData.StaticData.ItemName);
	float ApplyTime = RuntimeData.MedData.ApplyTime;
	if (UseElapsed >= ApplyTime)
	{
		FinishUse();
	}
}

void ACPP_EquipActor::Initialize(const FItemData& InItemData)
{
	GeneralData = InItemData;
	RuntimeData.StaticData = InItemData;
	RuntimeData.ItemQuantity = InItemData.ItemQuantity;
	RuntimeData.ItemWeight = RuntimeData.ItemQuantity == 1 ? InItemData.ItemWeight : InItemData.ItemWeight * InItemData.ItemQuantity;
	RuntimeData.MedData = InItemData.MedData;
	RuntimeData.WeaponData = InItemData.WeaponData;
	RuntimeData.MeeleeData = InItemData.MeeleeData;
	RuntimeData.AmmoData = InItemData.AmmoData;
	RuntimeData.ItemState = EItemState::Pool;
	OgTransform = GetActorTransform();
	if (GeneralData.EquippedMesh)
	{
		ItemMesh->SetSkeletalMesh(GeneralData.EquippedMesh);
	}
}

void ACPP_EquipActor::ResetRuntimeData()
{
	RuntimeData.StaticData = GeneralData;
	RuntimeData.ItemQuantity = GeneralData.ItemQuantity;
	RuntimeData.ItemWeight = RuntimeData.ItemQuantity == 1 ? GeneralData.ItemWeight : GeneralData.ItemWeight * GeneralData.ItemQuantity;
	RuntimeData.MedData = GeneralData.MedData;
	RuntimeData.WeaponData = GeneralData.WeaponData;
	RuntimeData.MeeleeData = GeneralData.MeeleeData;
	RuntimeData.AmmoData = GeneralData.AmmoData;
	RuntimeData.ItemState = EItemState::Pool;
	OwningCharacter = nullptr;
}

void ACPP_EquipActor::SetRuntimeData(const FItemRuntimeInfo& NewRuntimeData)
{
	RuntimeData = NewRuntimeData;
	//UE_LOG(LogTemp, Warning, TEXT("Value: %d"), RuntimeData.ItemQuantity);
}

void ACPP_EquipActor::ResetToPoolState()
{
	if (ItemMesh)
	{
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	SetActorTransform(OgTransform);
}