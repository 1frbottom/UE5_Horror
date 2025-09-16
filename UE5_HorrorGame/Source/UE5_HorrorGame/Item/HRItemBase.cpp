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

	//// implement destroying actor at child, 근데 지금 내가 인벤토리나 ui 보여주는게 전부 액터 참조 기반이라 destroy해버리면 아예 안돌아감. HRItemBase에서 인벤토리 어레이를 데이터 구조체 식으로 하는방법도 있는데, 인벤토리에서 액터 꺼내고 그런것도 생각하면 그냥 액터는 살아있는게 맞는거같음
	//// 기존 HRItemBase의 OnItemCollected를 대체

	//BP_OnPickedUp(character);

	if (character)
	{
		// 1. 아이템 스스로 플레이어의 인벤토리에 자신을 추가하도록 요청
		character->AddItemToInventory(this);

		// 2. 이후 공통 로직 수행
		bIsInteractable = false;
		OnFocusLost();
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);

		// 3. 블루프린트에서 처리할 추가 효과 호출
		BP_OnPickedUp(character);
	}
}

FText AHRItemBase::GetInteractionText_Implementation()
{
	return FText::FromString(TEXT("Pick Up"));
}