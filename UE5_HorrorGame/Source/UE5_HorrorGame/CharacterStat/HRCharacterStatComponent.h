// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HRCharacterStat.h"
#include "HRCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FHRCharacterStat& /*BaseStat*/, const FHRCharacterStat& /*ModifierStat*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStaminaChangedDelegate, float /*CurrentStamina*/);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE5_HORRORGAME_API UHRCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHRCharacterStatComponent();

protected:
	virtual void BeginPlay() override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;
	FOnStaminaChangedDelegate OnStaminaChanged;

	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	FORCEINLINE float GetCurrentStamina() { return CurrentStamina; }

	FORCEINLINE void SetBaseStat(const FHRCharacterStat& InBaseStat) { BaseStat = InBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void SetModifierStat(const FHRCharacterStat& InModifierStat) { ModifierStat = InModifierStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void AddBaseStat(const FHRCharacterStat& InAddBaseStat) { BaseStat = BaseStat + InAddBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }

	FORCEINLINE const FHRCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FHRCharacterStat& GetModifierStat() const { return ModifierStat; }

	FORCEINLINE FHRCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }

	FORCEINLINE void GainHp(float InHealAmount) { CurrentHp = FMath::Clamp(CurrentHp + InHealAmount, 0, GetTotalStat().MaxHp); OnHpChanged.Broadcast(CurrentHp); }
	float LoseHp(float InDamage);

	void UseStamina(float Amount);
	void RegenStamina(float Amount);

protected:
	void SetLevelStat(int32 InNewLevel);
	void SetHp(float NewHp);
	void SetStamina(float NewStamina);

	// provided by DataTable excel ( Singleton )
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat")
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat")
	float CurrentStamina;

	//UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	//float AttackRadius;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FHRCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FHRCharacterStat ModifierStat;

};