// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HRPlayerController.h"

#include "Character/HRCharacterPlayer.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


AHRPlayerController::AHRPlayerController()
{
	// UI Manager
	UIManager = CreateDefaultSubobject<UHRUIManagerComponent>(TEXT("UIManager"));

	static ConstructorHelpers::FObjectFinder<UInputAction> PauseActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/SystemAction/IA_PauseGame.IA_PauseGame'"));
	if (PauseActionRef.Object)
	{
		TogglePauseGameAction = PauseActionRef.Object;
	}

	// Error if using below : because of playercontroller called before asset loaded?
	// anyway, pick in controller BP
	//static ConstructorHelpers::FObjectFinder<UInputMappingContext> ContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_System.IMC_System'"));
	//if (ContextRef.Object)
	//{
	//	IMC_System = ContextRef.Object;
	//}



}

void AHRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(IMC_System, 0);
	}

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

void AHRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(TogglePauseGameAction, ETriggerEvent::Triggered, this, &AHRPlayerController::TogglePauseMenu);
	}


}

void AHRPlayerController::TogglePauseMenu()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("TogglePauseMenu() Called!"));
	}

	if (IsValid(PauseGameMenuWidgetClass))
	{
		UUserWidget* PauseMenuWidget = CreateWidget<UUserWidget>(this, PauseGameMenuWidgetClass);
		if (IsValid(PauseMenuWidget))
		{
			PauseMenuWidget->AddToViewport();
			SetPause(true);
			FInputModeUIOnly InputMode;
			SetInputMode(InputMode);
			bShowMouseCursor = true;
		}
	}
}


