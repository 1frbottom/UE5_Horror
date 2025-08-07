// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HRCharacterNonPlayer.h"
#include "Engine/AssetManager.h"
#include "AI/HRAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "Animation/AnimMontage.h"
#include "HRComboActionData.h"
#include "DrawDebugHelpers.h"	// attackhitcheck() debug



// Sets default values
AHRCharacterNonPlayer::AHRCharacterNonPlayer()
{
	// Stat
	CurrentHp = MaxHp;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;	// should randomize?

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NpcPawn"));

	//// Movement
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	//GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh ( test )
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("GhostMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/AssetUsing/JW/NonGit/NonFab/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/AssetUsing/JW/NonGit/NonFab/Mannequins/Animations/ABP_Manny.ABP_Manny_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// AI Control
	AIControllerClass = AHRAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

float AHRCharacterNonPlayer::GetAIPatrolRadius()
{
	return PatrolRadius;
}

float AHRCharacterNonPlayer::GetAIDetectRange()
{
	return DetectRange;
}

float AHRCharacterNonPlayer::GetAIAttackRange()
{
	// should return in more details
	// like, range + radius
	// range : perception range, radius : effective attack range

	return AttackRange + AttackRadius * 2;
}

float AHRCharacterNonPlayer::GetAITurnSpeed()
{
	return TurnSpeed;
}

void AHRCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AHRCharacterNonPlayer::AttackByAI()
{
	ProcessComboCommand();
}

void AHRCharacterNonPlayer::NotifycomboActionEnd()
{
	OnAttackFinished.ExecuteIfBound();
}

void AHRCharacterNonPlayer::ProcessComboCommand()
{
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}
}

void AHRCharacterNonPlayer::ComboActionBegin()
{
	// combo status
	CurrentCombo = 1;

	// movement setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
	const float AttackSpeedRate = 1.0f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	// when montage ended
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AHRCharacterNonPlayer::ComboActionEnd);

	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void AHRCharacterNonPlayer::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	NotifycomboActionEnd();
}

void AHRCharacterNonPlayer::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	// ���� �޺��� �̾����� �ִ� �ð�, �Ѿ�� �޺� �ʱ�ȭ
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeed;

	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AHRCharacterNonPlayer::ComboCheck, ComboEffectiveTime, false);
	}
}

void AHRCharacterNonPlayer::ComboCheck()
{
	ComboTimerHandle.Invalidate();

	if (CurrentCombo < ComboActionData->MaxComboCount)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);

		SetComboCheckTimer();
	}
}

void AHRCharacterNonPlayer::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	// �浹 ä���� ������Ʈ�� �°� Ȯ�� �� ���� �ʿ� (��: CCHANNEL_ABACTION)
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(AttackRadius), Params);

	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}


#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
}

float AHRCharacterNonPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// �߰����� ������ ó�� ������ ���⿡ ���� ����.
	// ex)
	// ���� �������� ���� Ư�� �ִϸ��̼� ���, ���� ȿ��, AI�� �ൿ ����
	// �������� ���� ���� (DamageCauser)�� ������ Ÿ�� (DamageEvent)�� ���� �ٸ� ����

	CurrentHp -= DamageAmount;
	if (CurrentHp <= 0.0f)
	{
		SetDead();
	}

	return DamageAmount;
}

void AHRCharacterNonPlayer::SetDead()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCharacterMovement()->StopMovementImmediately();
	// SetAIController(nullptr);
	PlayDeadAnimation();
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// AI ���� ���� �߰�
	AHRAIController* AIController = Cast<AHRAIController>(GetController());
	if (AIController)
	{
		AIController->StopAI();
	}

	// ���� �ð� �� Destroy ȣ�� ���� �߰�
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), 5.0f, false);

	// ���� �ð� ���� ��Ʈ ��Ȱ��ȭ?
}

void AHRCharacterNonPlayer::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}
