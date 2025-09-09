// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HRMainMenuHUD.h"

#include "Blueprint/UserWidget.h"

AHRMainMenuHUD::AHRMainMenuHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetAsset(TEXT("/Game/UI/WBP_MainMenu.WBP_MainMenu_C"));
	if (MainMenuWidgetAsset.Succeeded())
	{
		MainMenuWidgetClass = MainMenuWidgetAsset.Class;
	}

}

void AHRMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
		}
	}

}
