// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HRHpEffectWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE5_HORRORGAME_API UHRHpEffectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Effect")
	void UpdateHpEffect(float EffectAlpha);


};
