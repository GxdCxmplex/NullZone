// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseNPCController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "NavigationPath.h"
#include "NavigationSystemTypes.h"

#include "CPP_BaseNpc.h"
#include "NullZone/Data/NpcInfo.h"


ACPP_BaseNPCController::ACPP_BaseNPCController()
{
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	SetPerceptionComponent(*PerceptionComp);
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("SightConfig");
	SightConfig->SightRadius = 1500.f;
	SightConfig->LoseSightRadius = 1800.f;
	SightConfig->PeripheralVisionAngleDegrees = 70.f;
	SightConfig->SetMaxAge(5.f);

	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());

}

void ACPP_BaseNPCController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (!NPC_BT) return;
	RunBehaviorTree(NPC_BT);
	BlackboardComponent = GetBlackboardComponent();
}

void ACPP_BaseNPCController::OnDamaged(AActor* DamageCauser, FVector HitLocation)
{
	if (!BlackboardComponent) return;

	BlackboardComponent->SetValueAsBool(TEXT("WasDamagedRecenty"), true);
	BlackboardComponent->SetValueAsEnum(TEXT("GeneralState"), (uint8)ENpcGeneralState::Attacking);

	if (DamageCauser)
	{
		BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), DamageCauser);
		BlackboardComponent->SetValueAsBool(TEXT("HasTarget"), true);
		BlackboardComponent->SetValueAsVector(
			TEXT("PointOfinterest"),
			DamageCauser->GetActorLocation()
		);
	}
	else
	{
		BlackboardComponent->SetValueAsVector(TEXT("PointOfinterest"), HitLocation);
	}
}

FVector ACPP_BaseNPCController::GetRandomWanderLocation(float Radius)
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return FVector::ZeroVector;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys) return FVector::ZeroVector;

	FNavLocation NavLoc;
	if (NavSys->GetRandomReachablePointInRadius(ControlledPawn->GetActorLocation(), Radius, NavLoc))
	{
		return NavLoc.Location;
	}
	return ControlledPawn->GetActorLocation();
}

void ACPP_BaseNPCController::SetGeneralState(ENpcGeneralState NewState)
{
	if (!BlackboardComponent) return;

	BlackboardComponent->SetValueAsEnum("GeneralState", (uint8)NewState);

	switch (NewState)
	{
	case ENpcGeneralState::Passive:
		BlackboardComponent->SetValueAsVector("LastKnownLocation", FVector::ZeroVector);
		break;
	case ENpcGeneralState::Attacking:
		break;
	case ENpcGeneralState::Stunned:
		break;
	case ENpcGeneralState::Investigating:
		BlackboardComponent->ClearValue("TargetActor");
		BlackboardComponent->SetValueAsBool("HasTarget", false);
		break;
	case ENpcGeneralState::Dead:
		break;
	default:
		break;
	}
}

void ACPP_BaseNPCController::InitializePerceptionFromNPC(const FNpcStaticData& SD)
{
	if (!PerceptionComp || !SightConfig) return;

	SightConfig->SightRadius = SD.SightRadius;
	SightConfig->LoseSightRadius = SD.LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = SD.PeripheralVisionAngleDegrees;
	SightConfig->SetMaxAge(5.f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComp->RequestStimuliListenerUpdate();

	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ACPP_BaseNPCController::OnTargetPerceptionUpdated);

	SetGeneralState(ENpcGeneralState::Passive);
}

void ACPP_BaseNPCController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("Perception update: %s, Success: %d"), *Actor->GetName(), Stimulus.WasSuccessfullySensed());
	if (!Actor || !Actor->ActorHasTag("Player") || Actor == GetPawn()) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		BlackboardComponent->SetValueAsObject("TargetActor", Actor);
		BlackboardComponent->SetValueAsBool("HasTarget", true);
		BlackboardComponent->SetValueAsVector("PointOfinterest", Stimulus.StimulusLocation);
		BlackboardComponent->SetValueAsEnum("GeneralState", (uint8)ENpcGeneralState::Attacking);
	}
	else
	{
		BlackboardComponent->SetValueAsVector("PointOfinterest", Stimulus.StimulusLocation);
		BlackboardComponent->ClearValue("TargetActor");
		BlackboardComponent->SetValueAsBool("HasTarget", false);
		BlackboardComponent->SetValueAsEnum("GeneralState", (uint8)ENpcGeneralState::Investigating);
	}
}
