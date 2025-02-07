// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HRPlayerController.h"

void AHRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

}
