// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HRComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class UE5_HORRORGAME_API UHRComboActionData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UHRComboActionData();

	UPROPERTY(EditAnywhere, category = Name)
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, category = Name)
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, category = Name)
	float FrameRate;

	UPROPERTY(EditAnywhere, category = ComboData)
	TArray<float> EffectiveFrameCount;

};
