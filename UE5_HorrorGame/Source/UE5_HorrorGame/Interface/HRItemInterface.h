// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HRItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHRItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE5_HORRORGAME_API IHRItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool IsPickable() const = 0;

	virtual void OnPickedUp() = 0;			// what should happen after picked up


};
