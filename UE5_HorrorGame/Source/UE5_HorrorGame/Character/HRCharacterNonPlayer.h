// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HRCharacterAIinterface.h"
#include "Interface/HRAnimationInterface.h"
#include "Components/AudioComponent.h"

#include "HRCharacterNonPlayer.generated.h"

UCLASS()
class UE5_HORRORGAME_API AHRCharacterNonPlayer : public ACharacter, public IHRCharacterAIinterface, public IHRAnimationInterface
{
	GENERATED_BODY()

public:
	AHRCharacterNonPlayer();

protected:
	virtual void BeginPlay() override;

// Mesh
protected:

// Stat
protected:
	float MaxHp = 1000.0f;
	float CurrentHp;

	float AttackDamage = 30.0f;
	float AttackRange = 100.0f;
	float AttackSpeed = 1.0f;
	float AttackRadius = 50.0f;

	float PatrolRadius = 800.0f;
	float DetectRange = 400.0f;
	float TurnSpeed = 2.0f;

// Audio
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAudioComponent> HeartBeatSoundComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAudioComponent> GrowlingSoundComponent;

	float MinGrowlInterval = 7.0f;
	float MaxGrowlInterval = 21.0f;

private:
	FTimerHandle GrowlTimerHandle;
	void PlayGrowlSound();


// Animation 
protected:
	
	//// Combo Action Section
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	//TObjectPtr<class UAnimMontage> ComboActionMontage;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UHRComboActionData> ComboActionData; 

	//void ProcessComboCommand();
	//void ComboActionBegin();
	//void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	//void SetComboCheckTimer();
	//void ComboCheck();

	//int32 CurrentCombo = 0;
	//FTimerHandle ComboTimerHandle;
	//bool HasNextComboCommand = false;

	// Attack Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;
	void PlayAttackMontage();
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// Attack Hit Section
	void AttackHitCheck();
	virtual void DoAttackHitCheck_Implementation() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Dead Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	void SetDead();
	void PlayDeadAnimation();
	float DeadEventDelayTime = 5.0f;

// AI Section (interface)
protected:
	virtual float GetAIPatrolRadius()  override;
	virtual float GetAIDetectRange()  override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed()  override;

	FAICharacterAttackFinished OnAttackFinished;
	 virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;

	 virtual void AttackByAI() override;


	// void NotifycomboActionEnd();
	void NotifyAttackEnd();

};
