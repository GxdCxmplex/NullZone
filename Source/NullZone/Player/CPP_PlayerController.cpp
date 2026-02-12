// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerController.h"
#include "Blueprint/UserWidget.h"


ACPP_PlayerController::ACPP_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACPP_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController())
	{
		if (PlayerHUDClass)
		{
			HUD = CreateWidget<UCPP_PlayerHUD>(this, PlayerHUDClass);
			if (HUD)
			{
				HUD->AddToViewport();
				HUD->UpdateStamina(100, 100, 100);
				HUD->UpdateHealth(100, 100, 100);
			}
		}
		if (InventoryHUDClass)
		{
			InvHUD = CreateWidget<UCPP_InventoryHUD>(this, InventoryHUDClass);
			if (InvHUD)
			{
				InvHUD->AddToViewport();
			}
		}
	}
}

void ACPP_PlayerController::UpdateStaminaFunc(float Old, float Current, float Max)
{
	if (HUD)
	{
		HUD->UpdateStamina(Old, Current, Max);
	}
	OnStaminaChanged.Broadcast(Old, Current, Max);
}

void ACPP_PlayerController::UpdateHealthFunc(float Old, float Current, float Max)
{
	if (HUD)
	{
		HUD->UpdateHealth(Old, Current, Max);
	}
	OnHealthChanged.Broadcast(Old, Current, Max);
}


void ACPP_PlayerController::OnInventoryOpen(bool dowhat)
{
	if (InvHUD)
	{
		if (dowhat)
		{
			FInputModeGameAndUI GameAndUIMode;
			SetInputMode(GameAndUIMode);
		}
		else
		{
			FInputModeGameOnly GameOnlyMode;
			SetInputMode(GameOnlyMode);
		}
		InvHUD->OnInventoryButtonClicked(dowhat);
	}
}

void ACPP_PlayerController::ChangeInteractionText(const FString& NewText)
{
	if (HUD)
	{
		HUD->UpdateInteraction(NewText);
	}
}

void ACPP_PlayerController::WeaponWheelVisibilityHandler(bool bVisibility)
{
	if (InvHUD)
	{
		if (bVisibility)
		{
			FInputModeGameOnly GameOnlyMode;
			SetInputMode(GameOnlyMode);
		}
		InvHUD->VisibilityHandler(bVisibility);
	}
}

void ACPP_PlayerController::WeaponWheelLookEvent(float MouseX, float MouseY)
{
	if (InvHUD)
	{
		InvHUD->LookEvent(MouseX, MouseY);
	}
}

void ACPP_PlayerController::UpdateMainProgressBarPercent(float Percent, FString Name)
{
	if (HUD)
	{
		HUD->UpdateProgressBar(Percent, Name);
	}
}

int32 ACPP_PlayerController::GetCurrentWeaponSlot()
{
	if (InvHUD)
	{
		return InvHUD->GetCurrentSlot();
	}
	return -1;
}
