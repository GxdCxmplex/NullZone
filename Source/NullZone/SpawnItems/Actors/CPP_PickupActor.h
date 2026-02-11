// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NullZone/Data/ItemInfo.h"
#include "NullZone/Interactable.h"
#include "Engine/DataTable.h"
#include "CPP_PickupActor.generated.h"

UCLASS()
class NULLZONE_API ACPP_PickupActor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	ACPP_PickupActor();

	virtual ACPP_PickupActor* GetPickedUpData_Implementation() const override;
	virtual void OnPickedUp_Implementation() override;
	virtual void OnBeginHover_Implementation() override;
	virtual void OnEndHover_Implementation() override;

	void ResetRuntimeData();
	void Initialize(const FItemData& InItemData);
	void SetRuntimeData(const FItemRuntimeInfo& RuntimeData, EItemState NewState);
	void ResetToPoolState();

	void SetItemState(EItemState NewState);

	UFUNCTION(BlueprintCallable, Category = "Item")
	FItemData GetGeneralData() const { return GeneralData; }

	FORCEINLINE FItemRuntimeInfo GetRuntimeInfo() const { return RuntimeData; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemData GeneralData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemRuntimeInfo RuntimeData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	FGuid InstanceId;

	FTransform OgTransform;

};
