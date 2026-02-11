// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NullZone/Data/NpcInfo.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "CPP_BaseNPCController.generated.h"

/**
 * 
 */
UCLASS()
class NULLZONE_API ACPP_BaseNPCController : public AAIController
{
	GENERATED_BODY()
	
public:

	ACPP_BaseNPCController();

	virtual void OnPossess(APawn* InPawn) override;

	void OnDamaged(AActor* DamageCauser, FVector HitLocation);

	FVector GetRandomWanderLocation(float Radius);

	void SetGeneralState(ENpcGeneralState NewState);

	UFUNCTION()
	void InitializePerceptionFromNPC(const FNpcStaticData& SD);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* NPC_BT;

	UPROPERTY()
	class UBlackboardComponent* BlackboardComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* PerceptionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
