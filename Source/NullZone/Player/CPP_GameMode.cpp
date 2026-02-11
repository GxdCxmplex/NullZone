// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_GameMode.h"
#include "CPP_PlayerCharacter.h"
#include "CPP_PlayerController.h"

ACPP_GameMode::ACPP_GameMode()
{
	PlayerControllerClass = ACPP_PlayerController::StaticClass();
	DefaultPawnClass = ACPP_PlayerCharacter::StaticClass();
}

