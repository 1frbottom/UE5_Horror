// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HRMainMenuGameMode.h"

#include "Player/HRMainMenuPlayerController.h"
#include "UI/HRMainMenuHUD.h"
#include "GameFramework/Pawn.h"

AHRMainMenuGameMode::AHRMainMenuGameMode()
{
	PlayerControllerClass = AHRMainMenuPlayerController::StaticClass();
	HUDClass = AHRMainMenuHUD::StaticClass();

	DefaultPawnClass = APawn::StaticClass();


}
