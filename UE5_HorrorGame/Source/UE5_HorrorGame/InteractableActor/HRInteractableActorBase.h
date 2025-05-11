// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HRInteractableActorInterface.h"
#include "HRInteractableActorBase.generated.h"

UCLASS()
class UE5_HORRORGAME_API AHRInteractableActorBase : public AActor, public IHRInteractableActorInterface
{
	GENERATED_BODY()
	
public:	
	AHRInteractableActorBase();

protected:
	virtual void BeginPlay() override;

	// component?



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Info")
	bool bIsInteractable;

public:	
	virtual bool IsInteractable() const override { return bIsInteractable; }

};
