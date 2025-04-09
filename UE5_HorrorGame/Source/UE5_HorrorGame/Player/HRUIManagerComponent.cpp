// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HRUIManagerComponent.h"
#include "Blueprint/UserWidget.h"

// Sets default values for this component's properties
UHRUIManagerComponent::UHRUIManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UHRUIManagerComponent::SetInputModeUI(UUserWidget* Widget)
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(Widget->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);

        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;

        int32 ViewportX, ViewportY;
        PC->GetViewportSize(ViewportX, ViewportY);
        PC->SetMouseLocation(ViewportX / 2, ViewportY / 2);
    }
}

void UHRUIManagerComponent::SetInputModeGame()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }
}


