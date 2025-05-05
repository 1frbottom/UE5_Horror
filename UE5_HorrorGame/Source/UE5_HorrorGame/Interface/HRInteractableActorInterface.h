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

	///** �÷��̾ �� ���͸� ��Ŀ��(�ٶ󺸱� ����)���� �� ȣ��˴ϴ�. �ַ� ���̶���Ʈ ȿ���� �մϴ�. */
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	//void BeginFocus();

	///** �÷��̾ �� ���ͷκ��� ��Ŀ���� �Ҿ���(�ٸ� �� ���� ����) �� ȣ��˴ϴ�. �ַ� ���̶���Ʈ ȿ���� ���ϴ�. */
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	//void EndFocus();


	///** ��ȣ�ۿ� UI�� ǥ�õ� �ؽ�Ʈ�� ��ȯ�մϴ�. (��: "EŰ ���� �� ����") */
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	//FText GetInteractionPrompt() const;

};
