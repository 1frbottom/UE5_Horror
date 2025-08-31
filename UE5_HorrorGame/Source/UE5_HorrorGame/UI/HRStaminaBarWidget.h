// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HRStaminaBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE5_HORRORGAME_API UHRStaminaBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateStaminaBar(float CurrentStamina, float MaxStamina);

};
