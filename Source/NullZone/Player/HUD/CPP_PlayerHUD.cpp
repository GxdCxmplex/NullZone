// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerHUD.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/ProgressBar.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "NullZone/SpawnItems/CPP_WaveManager.h"


void UCPP_PlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	UMaterialInstanceDynamic* HealthBarMat = HealthBar->GetDynamicMaterial();
	UMaterialInstanceDynamic* HealthLowerBarMat = HealthBarLower->GetDynamicMaterial();
	UMaterialInstanceDynamic* StaminaBarMat = StaminaBar->GetDynamicMaterial();
	UMaterialInstanceDynamic* StaminaLowerBarMat = StaminaBarLower->GetDynamicMaterial();

	if (HealthBarMat)
	{
		HealthBarMID = HealthBarMat;
	}

	if (HealthLowerBarMat)
	{
		HealthLowerBarMID = HealthLowerBarMat;
	}

	if (StaminaBarMat)
	{
		StaminaBarMID = StaminaBarMat;
	}

	if (StaminaLowerBarMat)
	{
		StaminaLowerBarMID = StaminaLowerBarMat;
	}
	
	WaveManager = Cast<ACPP_WaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACPP_WaveManager::StaticClass()));
	if (!WaveManager)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get wave manager: CPP_PlayerHUD"));
		return;
	}
	WaveManager->OnWaveUpdateInfo.AddDynamic(this, &UCPP_PlayerHUD::WaveInfoUpdate);
	MainProgressBarBorder->SetVisibility(ESlateVisibility::Hidden);
}


void UCPP_PlayerHUD::UpdateStamina(float Old, float Current, float Max)
{
	if (!StaminaBar) return;
	StaminaBarMID->SetScalarParameterValue("Percentage", Current / Max);
	StaminaText->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%.0f"), Current, Max)));
	if (Old > Current)
	{
		StaminaCurrent = Current;
		StaminaOld = Old;
		StaminaTemp = Old;
		StaminaMax = Max;
		GetWorld()->GetTimerManager().ClearTimer(StaminaTimer);
		GetWorld()->GetTimerManager().SetTimer(StaminaTimer, this, &UCPP_PlayerHUD::StartStamina, .2f, false);
	}
	else
	{
		StaminaLowerBarMID->SetScalarParameterValue("Percentage", Current / Max);
		bIsChangingStamina = false;
	}
}


void UCPP_PlayerHUD::UpdateHealth(float Old, float Current, float Max)
{
	if (!HealthBar) return;
	HealthBarMID->SetScalarParameterValue("Percentage", Current / Max);
	HealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%.0f"), Current, Max)));
	if (Old > Current)
	{
		HealthCurrent = Current;
		HealthOld = Old;
		HealthTemp = Old;
		HealthMax = Max;
		GetWorld()->GetTimerManager().ClearTimer(HealthTimer);
		GetWorld()->GetTimerManager().SetTimer(HealthTimer, this, &UCPP_PlayerHUD::StartHealth, .2f, false);
	}
	else
	{
		HealthLowerBarMID->SetScalarParameterValue("Percentage", Current / Max);
		bIsChangingHealth = false;
	}
}

void UCPP_PlayerHUD::UpdateInteraction(const FString& NewText)
{
	InteractionText->SetText(FText::FromString(NewText));
}

void UCPP_PlayerHUD::WaveInfoUpdate(EWavePhase Phase, float TimeLeft, int32 WaveNum)
{
	WaveNumText->SetText(FText::Format(FText::FromString("Current Wave: {0}"), WaveNum));
	switch (Phase)
	{
	case EWavePhase::None:
		ObjectiveText->SetText(FText::FromString("Preparing"));
		break;
	case EWavePhase::Preparation:
	{
		ObjectiveText->SetText(FText::FromString("Objective: Scavenge for supplies"));
		int32 TotalSeconds = FMath::Max(0, FMath::FloorToInt(TimeLeft));
		int32 Minutes = TotalSeconds / 60;
		int32 Seconds = TotalSeconds % 60;
		FString TimeString = FString::Printf(TEXT("Remaining Time: %02d:%02d"), Minutes, Seconds);
		TimeText->SetText(FText::FromString(TimeString));
		break;
	}
	case EWavePhase::Wave:
	{
		ObjectiveText->SetText(FText::FromString("Objective: Survive"));
		TimeText->SetText(FText::FromString("Defeat All enemies"));
	}
		break;
	case EWavePhase::Cleanup:
	{
		ObjectiveText->SetText(FText::FromString("Preparing..."));
		TimeText->SetText(FText::FromString(""));
	}
		break;
	default:
		break;
	}
}

void UCPP_PlayerHUD::StartStamina()
{
	bIsChangingStamina = true;
	GetWorld()->GetTimerManager().ClearTimer(StaminaTimer);
}

void UCPP_PlayerHUD::StartHealth()
{
	bIsChangingHealth = true;
	GetWorld()->GetTimerManager().ClearTimer(HealthTimer);
}

void UCPP_PlayerHUD::UpdateProgressBar(float Percent, FString Name)
{
	if (Percent == 0.f or Percent == 1.f)
	{
		MainProgressBarBorder->SetVisibility(ESlateVisibility::Hidden);
		ProgressText->SetText(FText::FromString(""));
	}
	else
	{
		MainProgressBarBorder->SetVisibility(ESlateVisibility::Visible);
		MainProgressBar->SetPercent(Percent);
		ProgressText->SetText(FText::Format(FText::FromString("Using: {0}"), FText::FromString(Name)));
	}
}

void UCPP_PlayerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsChangingStamina)
	{
		StaminaTemp = FMath::FInterpTo(StaminaTemp, StaminaCurrent, InDeltaTime, 4.f);
		StaminaLowerBarMID->SetScalarParameterValue("Percentage", StaminaTemp / StaminaMax);

		if (FMath::IsNearlyEqual(StaminaTemp, StaminaCurrent, .1f))
		{
			StaminaTemp = StaminaCurrent;
			bIsChangingStamina = false;
		}
	}

	if (bIsChangingHealth)
	{
		HealthTemp = FMath::FInterpTo(HealthTemp, HealthCurrent, InDeltaTime, 4.f);
		HealthLowerBarMID->SetScalarParameterValue("Percentage", HealthTemp / HealthMax);

		if (FMath::IsNearlyEqual(HealthTemp, HealthCurrent, .1f))
		{
			HealthTemp = HealthCurrent;
			bIsChangingHealth = false;
		}
	}
}
