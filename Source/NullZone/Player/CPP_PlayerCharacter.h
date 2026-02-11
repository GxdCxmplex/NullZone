// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CPP_InventoryComponent.h"
#include "NullZone/Interactable.h"
#include "CPP_PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class ACPP_PlayerController;

UCLASS()
class NULLZONE_API ACPP_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACPP_PlayerCharacter();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class ACPP_PlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCPP_InventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* IMC_Default;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CrouchInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* WeaponWheelInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InventoryInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PickupInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MiscWheelInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* M1Input;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* GInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Health = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float HealthMax = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Stamina = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float StaminaMax = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Sanity = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float SanityMax = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Char")
	float WalkSpeed = 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Char")
	float CrouchSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float RunSpeed = 650.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float JumpCost = 15.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float StaminaRegenRate = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	bool bCanRegenStamina = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	bool bIsSprinting = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	bool bIsJumping = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	bool bEnableRegenBool = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	bool bIsCrouching = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	bool bCanJump = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FollowCamera;

	FTimerHandle RegenTimerHandle;
	FTimerHandle JumpCdTimerHandle;

protected:
	virtual void BeginPlay() override;

	void LookFunc(const FInputActionValue& value);
	void SprintStartFunc(const FInputActionValue& value);
	void SprintEndFunc(const FInputActionValue& value);
	void JumpFunc(const FInputActionValue& value);
	void CrouchStartFunc(const FInputActionValue& value);
	void CrouchEndFunc(const FInputActionValue& value);
	void WeaponWheelStartFunc(const FInputActionValue& value);
	void WeaponWheelEndFunc(const FInputActionValue& value);
	void InventoryFunc(const FInputActionValue& value);
	void OnPickupPressed(const FInputActionValue& value);
	void MiscWheelStartFunc(const FInputActionValue& value);
	void MiscWheelEndFunc(const FInputActionValue& value);
	void M1Down();
	void M1Up();
	void RKeyDown();
	void GKeyDown();


	void TraceForItems();

	UFUNCTION()
	void OnStaminaChanged(float Old, float Current, float Max);

	UFUNCTION()
	void OnHealthChanged(float Old, float Current, float Max);

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void EnableRegen();
	UFUNCTION()
	void JumpCd();

	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetHealthMax() const { return HealthMax; }

	void SetStamina(float Current, float Max);
	void SetHealth(float Current, float Max);

	void CanRegenStaminaFunc();
	
	UFUNCTION(BlueprintCallable)
	void TryPickUpItem();

	void ClearTargetItem();

	void MedApplied(FItemRuntimeInfo Data);
	void UpdateProgressBar(float Percent, FString Name);

	UFUNCTION(BlueprintCallable)

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE UCPP_InventoryComponent* GetInventoryComponent() const {return InventoryComponent;}

private:
	UPROPERTY()
	TScriptInterface<IInteractable> TargetItem;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	float PickupRange = 250.f;

	bool bIsLookingAtItem = false;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, Category = "UI")
	bool bInvVisibility = false;
	UPROPERTY(EditAnywhere, Category = "UI")
	bool bWeaponWheel = false;
	UPROPERTY(EditAnywhere, Category = "UI")
	bool bMiscWheel = false;

	UPROPERTY(EditAnywhere, Category = "UI")
	bool bM1Down = false;
};
