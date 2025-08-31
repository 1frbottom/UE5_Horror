// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/HRCharacterStatComponent.h"
#include "Game/HRGameSingleton.h"

// Sets default values for this component's properties
UHRCharacterStatComponent::UHRCharacterStatComponent()
{
	CurrentLevel = 1;
	//AttackRadius = 50.0f;
}

// Called when the game starts
void UHRCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
	SetStamina(BaseStat.Stamina);
}

void UHRCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UHRGameSingleton::Get().CharacterMaxLevel);

	SetBaseStat(UHRGameSingleton::Get().GetCharacterStat(CurrentLevel));

	check(BaseStat.MaxHp > 0.0f);
}

void UHRCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, GetTotalStat().MaxHp);

	OnHpChanged.Broadcast(CurrentHp);
}

float UHRCharacterStatComponent::LoseHp(float InDamage)
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

void UHRCharacterStatComponent::SetStamina(float NewStamina)
{
	CurrentStamina = FMath::Clamp<float>(NewStamina, 0.0f, GetTotalStat().Stamina);

	OnStaminaChanged.Broadcast(CurrentStamina);
}

void UHRCharacterStatComponent::UseStamina(float Amount)
{
	SetStamina(CurrentStamina - Amount);
}

void UHRCharacterStatComponent::RegenStamina(float Amount)
{
	SetStamina(CurrentStamina + Amount);
}