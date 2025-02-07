// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HRCharacterGhost.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AHRCharacterGhost::AHRCharacterGhost()
{
	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("GhostCapsule"));

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("GhostMesh"));

	//// input proper ghost skeletal mesh and animinstance
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> GhostMeshRef(TEXT(""));
	//if (GhostMeshRef.Object)
	//{
	//	GetMesh()->SetSkeletalMesh(GhostMeshRef.Object);
	//}
	//
	//static ConstructorHelpers::FObjectFinder<UAnimInstance> AnimInstanceClassRef(TEXT(""));
	//if (AnimInstanceClassRef.Class)
	//{
	//	GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	//}



}


