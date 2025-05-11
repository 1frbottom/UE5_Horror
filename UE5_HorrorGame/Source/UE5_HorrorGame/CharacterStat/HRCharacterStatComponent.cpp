// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/HRCharacterStatComponent.h"

// Sets default values for this component's properties
UHRCharacterStatComponent::UHRCharacterStatComponent()
{
	MaxHp = 100.0f;
	CurrentHp = MaxHp;
}


// Called when the game starts
void UHRCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHp(MaxHp);
}

float UHRCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UHRCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);

	OnHpChanged.Broadcast(CurrentHp);
}

