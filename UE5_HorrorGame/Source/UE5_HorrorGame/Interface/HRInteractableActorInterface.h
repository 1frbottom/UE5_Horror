// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HRInteractableActorInterface.generated.h"

class AHRCharacterPlayer;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHRInteractableActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE5_HORRORGAME_API IHRInteractableActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual bool IsInteractable() const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void BP_Interact(AHRCharacterPlayer* character);

	//--------------------------------------------------------------------------

	///** 플레이어가 이 액터를 포커스(바라보기 시작)했을 때 호출됩니다. 주로 하이라이트 효과를 켭니다. */
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	//void BeginFocus();

	///** 플레이어가 이 액터로부터 포커스를 잃었을(다른 곳 보기 시작) 때 호출됩니다. 주로 하이라이트 효과를 끕니다. */
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	//void EndFocus();


	///** 상호작용 UI에 표시될 텍스트를 반환합니다. (예: "E키 눌러 문 열기") */
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	//FText GetInteractionPrompt() const;

};
