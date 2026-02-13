// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"

#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#include "CPP_PlayerController.h"
#include "NullZone/SpawnItems/Actors/CPP_PickupActor.h"
#include "NullZone/SpawnItems/Actors/CPP_EquipActor.h"

#include "NullZone/Equippable.h"



// Sets default values
ACPP_PlayerCharacter::ACPP_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(35.0f, 90.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

	InventoryComponent = CreateDefaultSubobject<UCPP_InventoryComponent>(TEXT("InventoryComponent"));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FollowCamera->bUsePawnControlRotation = true;
	FollowCamera->SetupAttachment(GetMesh(), TEXT("head"));
	FollowCamera->SetRelativeLocation(FVector(0.f, 10.f, 0.f));
	FollowCamera->SetRelativeRotation(FRotator(-90.f, 0.f, 90.f));

	
}

void ACPP_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<ACPP_PlayerController>(GetController());

	if (PlayerController)
	{
		PlayerController->OnStaminaChanged.AddDynamic(this, &ACPP_PlayerCharacter::OnStaminaChanged);
		PlayerController->OnHealthChanged.AddDynamic(this, &ACPP_PlayerCharacter::OnHealthChanged);
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Default, 0);
		}
		PlayerController->ChangeInteractionText("");
	}
	InventoryComponent->InitializeInventory(ItemDataTable);

}




void ACPP_PlayerCharacter::OnStaminaChanged(float Old, float Current, float Max)
{
}

void ACPP_PlayerCharacter::OnHealthChanged(float Old, float Current, float Max)
{
}




void ACPP_PlayerCharacter::LookFunc(const FInputActionValue& value)
{
	const FVector2D LookAxisVector = value.Get<FVector2D>();
	if (bWeaponWheel)
	{
		if (PlayerController)
		{
			PlayerController->WeaponWheelLookEvent(LookAxisVector.X, LookAxisVector.Y);
		}
	}
	else if (!bInvVisibility)
	{
		AddControllerPitchInput(LookAxisVector.Y);
		AddControllerYawInput(LookAxisVector.X);
	}
}

void ACPP_PlayerCharacter::SprintStartFunc(const FInputActionValue& value)
{
		if (Stamina > 0.0f)
		{
			if (bIsCrouching)
			{
				UnCrouch();
				bIsCrouching = false;
			}
			bIsSprinting = true;
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

			GetWorldTimerManager().ClearTimer(RegenTimerHandle);
			bEnableRegenBool = false;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			bIsSprinting = false;
			GetWorldTimerManager().ClearTimer(RegenTimerHandle);
			bEnableRegenBool = false;
			GetWorldTimerManager().SetTimer(RegenTimerHandle, this, &ACPP_PlayerCharacter::EnableRegen, 1.5f, false);
		}
}

void ACPP_PlayerCharacter::SprintEndFunc(const FInputActionValue& value)
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	GetWorldTimerManager().ClearTimer(RegenTimerHandle);
	bEnableRegenBool = false;
	GetWorldTimerManager().SetTimer(RegenTimerHandle, this, &ACPP_PlayerCharacter::EnableRegen, 1.5f, false);
}

void ACPP_PlayerCharacter::CrouchStartFunc(const FInputActionValue& value)
{
		if (bIsSprinting)
		{
			bIsSprinting = false;
			GetWorldTimerManager().ClearTimer(RegenTimerHandle);

			bEnableRegenBool = false;
			GetWorldTimerManager().SetTimer(RegenTimerHandle, this, &ACPP_PlayerCharacter::EnableRegen, 1.5f, false);

		}
		Crouch();
		bIsCrouching = true;
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
}

void ACPP_PlayerCharacter::CrouchEndFunc(const FInputActionValue& value)
{
	UnCrouch();
	bIsCrouching = false;
	if (bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}



void ACPP_PlayerCharacter::WeaponWheelStartFunc(const FInputActionValue& value)
{
		if (PlayerController)
		{
			bWeaponWheel = true;
			bInvVisibility = false;
			PlayerController->WeaponWheelVisibilityHandler(true);
			int32 SizeX, SizeY;
			PlayerController->GetViewportSize(SizeX, SizeY);
			PlayerController->SetMouseLocation(SizeX / 2, SizeY / 2);
			PlayerController->SetShowMouseCursor(true);
		}
}

void ACPP_PlayerCharacter::WeaponWheelEndFunc(const FInputActionValue& value)
{
	if (PlayerController)
	{
		bWeaponWheel = false;
		PlayerController->WeaponWheelVisibilityHandler(false);
		PlayerController->SetShowMouseCursor(bInvVisibility ? true : false);
		int32 CurrentSlot = PlayerController->GetCurrentWeaponSlot();
		InventoryComponent->EquipSlot(CurrentSlot - 1);
	}
}

void ACPP_PlayerCharacter::InventoryFunc(const FInputActionValue& value)
{
	PlayerController->OnInventoryOpen(!bInvVisibility);
	PlayerController->SetShowMouseCursor(bInvVisibility ? false : true);
	bInvVisibility = !bInvVisibility;
	bWeaponWheel = false;
	int32 SizeX, SizeY;
	PlayerController->GetViewportSize(SizeX, SizeY);
	PlayerController->SetMouseLocation(SizeX / 2, SizeY / 2);
}

void ACPP_PlayerCharacter::OnPickupPressed(const FInputActionValue& value)
{
	if (TargetItem)
	{
		TryPickUpItem();
	}
}

void ACPP_PlayerCharacter::M1Down()
{
	if (bWeaponWheel || bInvVisibility) return;
	ACPP_EquipActor* EquippedActor = InventoryComponent->GetEquippedActor();
	if (!EquippedActor) return;
	bM1Down = true;
	IEquippable::Execute_StartUseM1(Cast<UObject>(EquippedActor), this);
}

void ACPP_PlayerCharacter::M1Up()
{
	bM1Down = false;
	if (bWeaponWheel || bInvVisibility) return;
	ACPP_EquipActor* EquippedActor = InventoryComponent->GetEquippedActor();
	if (!EquippedActor) return;
	IEquippable::Execute_StopUseM1(Cast<UObject>(EquippedActor));
}

void ACPP_PlayerCharacter::M2Down()
{
	if (bWeaponWheel || bInvVisibility) return;
	ACPP_EquipActor* EquippedActor = InventoryComponent->GetEquippedActor();
	if (!EquippedActor) return;
	IEquippable::Execute_StartUseM2(Cast<UObject>(EquippedActor), this);
}

void ACPP_PlayerCharacter::M2Up()
{
	if (bWeaponWheel || bInvVisibility) return;
	ACPP_EquipActor* EquippedActor = InventoryComponent->GetEquippedActor();
	if (!EquippedActor) return;
	IEquippable::Execute_StopUseM2(Cast<UObject>(EquippedActor));
}

void ACPP_PlayerCharacter::RKeyDown()
{
	ACPP_EquipActor* EquippedActor = InventoryComponent->GetEquippedActor();
	if (!EquippedActor || bInvVisibility) return;
	EquippedActor->Reloading(this);
}

void ACPP_PlayerCharacter::GKeyDown()
{
	ACPP_EquipActor* EquippedActor = InventoryComponent->GetEquippedActor();
	if (!EquippedActor || bInvVisibility) return;
	EquippedActor->Unloading(this);
}

void ACPP_PlayerCharacter::MedApplied(FItemRuntimeInfo Data)
{
	InventoryComponent->UseItem(Data.StaticData.ItemId);
}

void ACPP_PlayerCharacter::UpdateProgressBar(float Percent, FString Name)
{
	if (!PlayerController) return;
	PlayerController->UpdateMainProgressBarPercent(Percent, Name);
}

void ACPP_PlayerCharacter::TraceForItems()
{
	if (!FollowCamera) return;

	FVector CameraLocation = FollowCamera->GetComponentLocation();
	FVector CameraForward = FollowCamera->GetForwardVector();
	FVector TraceEnd = CameraLocation + (CameraForward * PickupRange);

	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActor(this);

	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility, TraceParams);

/*#if !UE_BUILD_SHIPPING
	DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, bHit ? FColor::Green : FColor::Red, false, .0f, 0, 1.f);
#endif*/

	if (bHit && HitResult.GetActor())
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor->Implements<UInteractable>())
		{

			TScriptInterface<IInteractable> NewTarget;
			NewTarget.SetObject(HitActor);
			NewTarget.SetInterface(Cast<IInteractable>(HitActor));
			if (TargetItem != NewTarget)
			{
				if (TargetItem)
				{
					IInteractable::Execute_OnEndHover(TargetItem.GetObject());
				}

				TargetItem = NewTarget;

				if (TargetItem)
				{
					IInteractable::Execute_OnBeginHover(TargetItem.GetObject());
					PlayerController->ChangeInteractionText(IInteractable::Execute_GetPickedUpData(TargetItem.GetObject())->GetRuntimeInfo().StaticData.ItemName);
				}

				bIsLookingAtItem = true;

			}
		}
		else
		{
			ClearTargetItem();
		}
	}
	else
	{
		ClearTargetItem();
	}
}

void ACPP_PlayerCharacter::ClearTargetItem()
{
	if (TargetItem)
	{
		IInteractable::Execute_OnEndHover(TargetItem.GetObject());
		TargetItem = nullptr;
		bIsLookingAtItem = false;
		PlayerController->ChangeInteractionText("");
	}
}


void ACPP_PlayerCharacter::JumpFunc(const FInputActionValue& value)
{
	if (Stamina < JumpCost || !bCanJump) return;

	if (bIsCrouching)
	{
		UnCrouch();
		bIsCrouching = false;
	}
	Jump();
	bIsJumping = true;

	SetStamina(Stamina - JumpCost, StaminaMax);

	GetWorldTimerManager().ClearTimer(RegenTimerHandle);
	GetWorldTimerManager().ClearTimer(JumpCdTimerHandle);
	bEnableRegenBool = false;
	bCanJump = false;
	GetWorldTimerManager().SetTimer(RegenTimerHandle, this, &ACPP_PlayerCharacter::EnableRegen, 1.3f, false);
	GetWorldTimerManager().SetTimer(JumpCdTimerHandle, this, &ACPP_PlayerCharacter::JumpCd, 0.95f, false);
}



void ACPP_PlayerCharacter::EnableRegen()
{
	bEnableRegenBool = true;
}

void ACPP_PlayerCharacter::JumpCd()
{
	bCanJump = true;
	bIsJumping = false;
}

void ACPP_PlayerCharacter::SetStamina(float Current, float Max)
{
	float old = Stamina;
	Stamina = Current;
	if (PlayerController)
	{
		PlayerController->UpdateStaminaFunc(old, Current, Max);
	}
}

void ACPP_PlayerCharacter::SetHealth(float Current, float Max)
{
	float old = Health;
	Health = Current;
	if (PlayerController)
	{
		PlayerController->UpdateHealthFunc(old, Current, Max);
	}
}

void ACPP_PlayerCharacter::CanRegenStaminaFunc()
{
	if (!bIsSprinting && !bIsJumping)
	{
		bCanRegenStamina = true;
	}
	else
	{
		bCanRegenStamina = false;
	}
}

void ACPP_PlayerCharacter::TryPickUpItem()
{
	if (!TargetItem || !InventoryComponent) return;
	const ACPP_PickupActor* PickupActor = IInteractable::Execute_GetPickedUpData(TargetItem.GetObject());
	int32 bPicked = InventoryComponent->AddItem(PickupActor);
	if (bPicked > 0)
	{
		IInteractable::Execute_OnPickedUp(TargetItem.GetObject());
		ClearTargetItem();
		TargetItem = nullptr;
	}
		
}

void ACPP_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForItems();

	if (Stamina <= StaminaMax)
	{
		CanRegenStaminaFunc();
		if (bCanRegenStamina && bEnableRegenBool)
		{
			SetStamina(FMath::Min(Stamina + (StaminaRegenRate * DeltaTime), StaminaMax), StaminaMax);
		}
	}
}

void ACPP_PlayerCharacter::TakeDamage_Implementation(float Damage, AActor* DamageCauser)
{
	SetHealth(FMath::Min(Health - Damage, 0), HealthMax);
}

void ACPP_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookInput, ETriggerEvent::Triggered, this, &ACPP_PlayerCharacter::LookFunc);
		EnhancedInputComponent->BindAction(SprintInput, ETriggerEvent::Started, this, &ACPP_PlayerCharacter::SprintStartFunc);
		EnhancedInputComponent->BindAction(SprintInput, ETriggerEvent::Completed, this, &ACPP_PlayerCharacter::SprintEndFunc);
		EnhancedInputComponent->BindAction(JumpInput, ETriggerEvent::Triggered, this, &ACPP_PlayerCharacter::JumpFunc);
		EnhancedInputComponent->BindAction(CrouchInput, ETriggerEvent::Started, this, &ACPP_PlayerCharacter::CrouchStartFunc);
		EnhancedInputComponent->BindAction(CrouchInput, ETriggerEvent::Completed, this, &ACPP_PlayerCharacter::CrouchEndFunc);
		EnhancedInputComponent->BindAction(WeaponWheelInput, ETriggerEvent::Started, this, &ACPP_PlayerCharacter::WeaponWheelStartFunc);
		EnhancedInputComponent->BindAction(WeaponWheelInput, ETriggerEvent::Completed, this, &ACPP_PlayerCharacter::WeaponWheelEndFunc);
		EnhancedInputComponent->BindAction(InventoryInput, ETriggerEvent::Triggered, this, &ACPP_PlayerCharacter::InventoryFunc);
		EnhancedInputComponent->BindAction(PickupInput, ETriggerEvent::Started, this, &ACPP_PlayerCharacter::OnPickupPressed);
		EnhancedInputComponent->BindAction(M1Input, ETriggerEvent::Started, this, &ACPP_PlayerCharacter::M1Down);
		EnhancedInputComponent->BindAction(M1Input, ETriggerEvent::Completed, this, &ACPP_PlayerCharacter::M1Up);
		EnhancedInputComponent->BindAction(M2Input, ETriggerEvent::Started, this, &ACPP_PlayerCharacter::M2Down);
		EnhancedInputComponent->BindAction(M2Input, ETriggerEvent::Completed, this, &ACPP_PlayerCharacter::M2Up);
		EnhancedInputComponent->BindAction(RInput, ETriggerEvent::Triggered, this, &ACPP_PlayerCharacter::RKeyDown);
		EnhancedInputComponent->BindAction(GInput, ETriggerEvent::Triggered, this, &ACPP_PlayerCharacter::GKeyDown);
	}

}

