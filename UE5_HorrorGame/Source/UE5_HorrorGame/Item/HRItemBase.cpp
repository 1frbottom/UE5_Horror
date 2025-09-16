// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HRItemBase.h"
#include "Character/HRCharacterPlayer.h"

// Sets default values
AHRItemBase::AHRItemBase()
{
	TraceHitBox->SetBoxExtent(FVector(20.f));


}

void AHRItemBase::OnPickedUp(AHRCharacterPlayer* character)
{
	//bIsInteractable = false;

	//OnFocusLost();

	//SetActorEnableCollision(false);		// collision
	//SetActorHiddenInGame(true);			// visibility

	//// implement destroying actor at child, �ٵ� ���� ���� �κ��丮�� ui �����ִ°� ���� ���� ���� ����̶� destroy�ع����� �ƿ� �ȵ��ư�. HRItemBase���� �κ��丮 ��̸� ������ ����ü ������ �ϴ¹���� �ִµ�, �κ��丮���� ���� ������ �׷��͵� �����ϸ� �׳� ���ʹ� ����ִ°� �´°Ű���
	//// ���� HRItemBase�� OnItemCollected�� ��ü

	//BP_OnPickedUp(character);

	if (character)
	{
		// 1. ������ ������ �÷��̾��� �κ��丮�� �ڽ��� �߰��ϵ��� ��û
		character->AddItemToInventory(this);

		// 2. ���� ���� ���� ����
		bIsInteractable = false;
		OnFocusLost();
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);

		// 3. �������Ʈ���� ó���� �߰� ȿ�� ȣ��
		BP_OnPickedUp(character);
	}
}

FText AHRItemBase::GetInteractionText_Implementation()
{
	return FText::FromString(TEXT("Pick Up"));
}