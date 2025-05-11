// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HRUIManagerComponent.h"
#include "HRPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UE5_HORRORGAME_API AHRPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AHRPlayerController();

protected:
	virtual void BeginPlay() override;

// UI Manager
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UHRUIManagerComponent* UIManager;
};
