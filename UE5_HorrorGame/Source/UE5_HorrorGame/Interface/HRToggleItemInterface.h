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

	// �� �Լ��� BP���� ���� (BlueprintImplementableEvent) �ϰų�,
    // C++������ �⺻ ������ �����ϰ� BP���� �������̵� (BlueprintNativeEvent) �� �� �ֽ��ϴ�.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interact")
	void BP_ToggleItem();
};
