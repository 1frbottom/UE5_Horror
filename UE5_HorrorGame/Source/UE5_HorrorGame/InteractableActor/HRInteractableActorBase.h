// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HRInteractableActorInterface.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "HRInteractableActorBase.generated.h"

UCLASS()
class UE5_HORRORGAME_API AHRInteractableActorBase : public AActor, public IHRInteractableActorInterface
{
	GENERATED_BODY()
	
public:	
	AHRInteractableActorBase();

protected:
	virtual void BeginPlay() override;

// basic
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Info")
	bool bIsInteractable;

public:	
	virtual bool IsInteractable() const override { return bIsInteractable; }

// interaction prompt
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction UI")
	UWidgetComponent* InteractionPromptWidget;		// 액터의 컴포넌트로 들어가는거라 액터와 동시생성

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> InteractionWidgetAnchor;	// 모듈화 : 위젯컴포넌트 담을 앵커

	// do hardcoding within each BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction UI")
	FText InteractionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction UI")
	UBoxComponent* InteractionBox;

	UFUNCTION()
	void OnInteractionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnInteractionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnFocusGained();
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnFocusLost();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FText GetInteractionText();
	virtual FText GetInteractionText_Implementation();
	
		// for real time update
		// put this function between main interaction logic on BP
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void UpdateInteractionWidgetText();
};
