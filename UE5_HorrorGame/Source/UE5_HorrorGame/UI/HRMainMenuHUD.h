// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "HRMainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class UE5_HORRORGAME_API AHRMainMenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AHRMainMenuHUD();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;

	UPROPERTY()
	class UUserWidget* MainMenuWidget;


};
