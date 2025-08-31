// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"

#include "HRAIController.generated.h"

/**
 * 
 */
UCLASS()
class UE5_HORRORGAME_API AHRAIController : public AAIController
{
	GENERATED_BODY()

public:
	AHRAIController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UAIPerceptionComponent* AIPerceptionComp;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBasset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTasset;
};
