// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HRToggleItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHRToggleItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE5_HORRORGAME_API IHRToggleItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// 이 함수를 BP에서 구현 (BlueprintImplementableEvent) 하거나,
    // C++에서도 기본 구현을 제공하고 BP에서 오버라이드 (BlueprintNativeEvent) 할 수 있습니다.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interact")
	void BP_ToggleItem();
};
