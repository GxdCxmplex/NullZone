// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NullZone/Data/ItemInfo.h"
#include "CPP_PlayerHUD.generated.h"

class UImage;
class UMaterialInstanceDynamic;
class UTextBlock;
class ACPP_WaveManager;
class UBorder;
class UProgressBar;

UCLASS()
class NULLZONE_API UCPP_PlayerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	
	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateStamina(float Old, float Current, float Max);
	
	UFUNCTION(BlueprintCallable)
	void UpdateHealth(float Old, float Current, float Max);

	UFUNCTION(BlueprintCallable)
	void UpdateInteraction(const FString& NewText);

	UFUNCTION(BlueprintCallable)
	void WaveInfoUpdate(EWavePhase Phase, float TimeLeft, int32 WaveNum);

	void StartStamina();
	void StartHealth();

	void UpdateProgressBar(float Percent, FString Name);

public:

	FTimerHandle StaminaTimer;
	FTimerHandle HealthTimer;

protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* StaminaText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* StaminaBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* StaminaBarLower;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* HealthBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* HealthBarLower;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* InteractionText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WaveNumText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ObjectiveText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TimeText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* MainProgressBarBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* MainProgressBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ProgressText;

	UPROPERTY()
	UMaterialInstanceDynamic* HealthBarMID;
	UPROPERTY()
	UMaterialInstanceDynamic* HealthLowerBarMID;

	UPROPERTY()
	UMaterialInstanceDynamic* StaminaBarMID;
	UPROPERTY()
	UMaterialInstanceDynamic* StaminaLowerBarMID;

private:
	float StaminaTemp;
	float StaminaCurrent;
	float StaminaMax;
	float StaminaOld;
	bool bIsChangingStamina = false;

	float HealthTemp;
	float HealthCurrent;
	float HealthMax;
	float HealthOld;
	bool bIsChangingHealth = false;

	ACPP_WaveManager* WaveManager;
};
