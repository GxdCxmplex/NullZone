// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PickupActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ACPP_PickupActor::ACPP_PickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	ItemMesh->SetupAttachment(Root);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionSphere"));
	CollisionSphere->SetupAttachment(ItemMesh);
	CollisionSphere->SetSphereRadius(50.f);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMesh->SetCollisionProfileName(TEXT("PhysicActor"));
	ItemMesh->SetSimulatePhysics(false);
	ItemMesh->SetGenerateOverlapEvents(true);

}

ACPP_PickupActor* ACPP_PickupActor::GetPickedUpData_Implementation() const
{
	return const_cast<ACPP_PickupActor*>(this);
}

void ACPP_PickupActor::OnPickedUp_Implementation()
{
}

void ACPP_PickupActor::OnBeginHover_Implementation()
{
}

void ACPP_PickupActor::OnEndHover_Implementation()
{
}

void ACPP_PickupActor::Initialize(const FItemData& InItemData)
{
	GeneralData = InItemData;
	RuntimeData.StaticData = InItemData;
	RuntimeData.ItemQuantity = InItemData.ItemQuantity;
	RuntimeData.ItemWeight = RuntimeData.ItemQuantity == 1 ? InItemData.ItemWeight : InItemData.ItemWeight * InItemData.ItemQuantity;
	RuntimeData.MedData = InItemData.MedData;
	RuntimeData.WeaponData = InItemData.WeaponData;
	RuntimeData.MeeleeData = InItemData.MeeleeData;
	RuntimeData.AmmoData = InItemData.AmmoData;
	InstanceId = FGuid::NewGuid();
	RuntimeData.InstanceId = InstanceId;
	RuntimeData.ItemState = EItemState::Pool;

	if (GeneralData.PickupMesh)
	{
		ItemMesh->SetStaticMesh(GeneralData.PickupMesh);
	}
	OgTransform = GetActorTransform();
}

void ACPP_PickupActor::ResetRuntimeData()
{
	RuntimeData.StaticData = GeneralData;
	RuntimeData.ItemQuantity = GeneralData.ItemQuantity;
	RuntimeData.ItemWeight = RuntimeData.ItemQuantity == 1 ? GeneralData.ItemWeight : GeneralData.ItemWeight * GeneralData.ItemQuantity;
	RuntimeData.MedData = GeneralData.MedData;
	RuntimeData.WeaponData = GeneralData.WeaponData;
	RuntimeData.MeeleeData = GeneralData.MeeleeData;
	RuntimeData.AmmoData = GeneralData.AmmoData;
	InstanceId = FGuid::NewGuid();
	RuntimeData.InstanceId = InstanceId;
	RuntimeData.ItemState = EItemState::Pool;
}

void ACPP_PickupActor::SetRuntimeData(const FItemRuntimeInfo& NewRuntimeData, EItemState NewState)
{
	RuntimeData = NewRuntimeData;
	RuntimeData.ItemState = NewState;
	RuntimeData.InstanceId = NewRuntimeData.InstanceId;
	//UE_LOG(LogTemp, Warning, TEXT("Value: %d"), RuntimeData.ItemQuantity);
}

void ACPP_PickupActor::ResetToPoolState()
{
	if (ItemMesh)
	{
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	// Reset to original transform
	SetActorTransform(OgTransform);
}

void ACPP_PickupActor::SetItemState(EItemState NewState)
{
	RuntimeData.ItemState = NewState;
}


// Called when the game starts or when spawned
void ACPP_PickupActor::BeginPlay()
{
	Super::BeginPlay();
}


