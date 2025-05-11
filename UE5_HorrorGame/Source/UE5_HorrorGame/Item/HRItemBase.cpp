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

	// implement destroying actor at child, �ٵ� ���� ���� �κ��丮�� ui �����ִ°� ���� ���� ���� ����̶� destroy�ع����� �ƿ� �ȵ��ư�. HRItemBase���� �κ��丮 ��̸� ������ ����ü ������ �ϴ¹���� �ִµ�, �κ��丮���� ���� ������ �׷��͵� �����ϸ� �׳� ���ʹ� ����ִ°� �´°Ű���
	// ���� HRItemBase�� OnItemCollected�� ��ü
	BP_OnPickedUp(character);
}




