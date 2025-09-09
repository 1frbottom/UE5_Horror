// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HRPlayerController.h"

#include "Character/HRCharacterPlayer.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"



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

void AHRPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AHRCharacterPlayer* PlayerCharacter = Cast<AHRCharacterPlayer>(InPawn);
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->OnPlayerDied.AddDynamic(this, &AHRPlayerController::HandlePlayerDied);

	}
}

void AHRPlayerController::HandlePlayerDied()
{
	if (APawn* MyPawn = GetPawn())
	{
		MyPawn->DisableInput(this);
	}

	if (IsValid(GameOverWidgetClass))
	{
		UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(this, GameOverWidgetClass);
		if (IsValid(GameOverWidget))
		{
			GameOverWidget->AddToViewport();
		}
	}

	//// UnComment if you want "press 'E' to retry", also WBP_GameOver's event construct
	//FInputModeUIOnly InputMode;
	//SetInputMode(InputMode);

	GetWorld()->GetTimerManager().SetTimer(RestartTimerHandle, this, &AHRPlayerController::RetryLevel, RestartDelay, false);

}

void AHRPlayerController::RetryLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}