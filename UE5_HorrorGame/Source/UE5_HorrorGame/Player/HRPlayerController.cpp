// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HRPlayerController.h"

AHRPlayerController::AHRPlayerController()
{
	// UI Manager
	UIManager = CreateDefaultSubobject<UHRUIManagerComponent>(TEXT("UIManager"));

}

void AHRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

}
