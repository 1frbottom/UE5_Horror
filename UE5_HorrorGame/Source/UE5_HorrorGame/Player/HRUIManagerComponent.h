// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HRUIManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_HORRORGAME_API UHRUIManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHRUIManagerComponent();

public:
	UFUNCTION(BlueprintCallable)
	void SetInputModeUICustom(UUserWidget* Widget, bool bShowCursor);

	UFUNCTION(BlueprintCallable)
	void SetInputModeGameCustom();

	//// for overloading
	//UFUNCTION(BlueprintCallable)
	//void SetInputModeUI(UUserWidget* Widget, bool bLockCursor, bool bCenterMouse);

	//UFUNCTION(BlueprintCallable)
	//void CloseAllUI();

	UFUNCTION(BlueprintPure, Category = "UI")
	bool IsUIActive() const { return bIsUIActive; }

private:
	bool bIsUIActive = false;

};
