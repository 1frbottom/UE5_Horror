// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HRUIManagerComponent.h"
#include "Blueprint/UserWidget.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UHRUIManagerComponent::UHRUIManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UHRUIManagerComponent::SetInputModeUICustom(UUserWidget* Widget, bool bShowCursor)
{
    if (APlayerController* pController = GetWorld()->GetFirstPlayerController())
    {
        pController->StopMovement();

        // allow 'game input + ui input'
        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(Widget->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

        pController->SetInputMode(InputMode);
        pController->bShowMouseCursor = bShowCursor;

        int32 ViewportX, ViewportY;
        pController->GetViewportSize(ViewportX, ViewportY);
        pController->SetMouseLocation(ViewportX / 2.0f, ViewportY / 2.0f);

        bIsUIActive = true;
    }
}

void UHRUIManagerComponent::SetInputModeGameCustom()
{
    if (APlayerController* pController = GetWorld()->GetFirstPlayerController())
    {
        pController->SetInputMode(FInputModeGameOnly());
        pController->bShowMouseCursor = false;

        bIsUIActive = false;
    }
}


