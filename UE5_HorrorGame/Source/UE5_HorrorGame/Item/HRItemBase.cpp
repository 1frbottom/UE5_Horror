// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HRItemBase.h"

// Sets default values
AHRItemBase::AHRItemBase()
{

}

void AHRItemBase::OnPickedUp(AHRCharacterPlayer* character)
{
	SetActorEnableCollision(false);		// collision
	SetActorHiddenInGame(true);			// visibility

	// implement destroying actor at child, 근데 지금 내가 인벤토리나 ui 보여주는게 전부 액터 참조 기반이라 destroy해버리면 아예 안돌아감. HRItemBase에서 인벤토리 어레이를 데이터 구조체 식으로 하는방법도 있는데, 인벤토리에서 액터 꺼내고 그런것도 생각하면 그냥 액터는 살아있는게 맞는거같음
	// 기존 HRItemBase의 OnItemCollected를 대체
	BP_OnPickedUp(character);
}




