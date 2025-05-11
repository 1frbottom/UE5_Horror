// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HRWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class UE5_HORRORGAME_API UHRWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	virtual void InitWidget() override;


};
