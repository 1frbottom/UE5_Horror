// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HRUIManagerComponent.h"

#include "HRPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

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

	virtual void SetupInputComponent() override;

// UI Manager
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UHRUIManagerComponent* UIManager;
	
// SetDead Delegate
protected:
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void HandlePlayerDied();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameOver")
	TSubclassOf<class UUserWidget> GameOverWidgetClass;
	
	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void RetryLevel();

	UPROPERTY(EditDefaultsOnly, Category = "GameOver")
	float RestartDelay = 5.0f;

private:
	FTimerHandle RestartTimerHandle;

// Pause Menu
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> PauseGameMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> TogglePauseGameAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_System;

	void TogglePauseMenu();




};
