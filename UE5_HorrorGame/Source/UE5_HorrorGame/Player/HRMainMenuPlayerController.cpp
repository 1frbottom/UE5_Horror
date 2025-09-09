// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HRMainMenuPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

void AHRMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);

	ACameraActor* MainMenuCamera = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass()));
	if (MainMenuCamera)
	{
		SetViewTargetWithBlend(MainMenuCamera);
	}


}
