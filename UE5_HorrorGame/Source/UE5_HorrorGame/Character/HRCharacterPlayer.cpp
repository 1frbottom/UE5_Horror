// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HRCharacterPlayer.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Item/HRItemBase.h"
#include "InteractableActor/HRInteractableActorBase.h"

#include "Interface/HRToggleItemInterface.h"

#include "Components/GridPanel.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "UMG.h"	// ugridslot

#include "HRCharacterControlData.h"

#include "CharacterStat/HRCharacterStatComponent.h"
#include "UI/HRWidgetComponent.h"
#include "UI/HRHpBarWidget.h"




AHRCharacterPlayer::AHRCharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 125.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn_1"));

	// Mesh ( test )
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/NonFab/DefaultMannequin/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Asset/NonFab/DefaultMannequin/Mannequins/Animations/ABP_Manny.ABP_Manny_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// Camera

	CurrCharacterControlType = ECharacterControlType::First;

		// fpv
	SpringArm_Fpv = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm_Fpv"));
	SpringArm_Fpv->SetupAttachment(RootComponent);
	Camera_Fpv = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera_Fpv"));
	Camera_Fpv->SetupAttachment(SpringArm_Fpv, USpringArmComponent::SocketName);

	SpringArm_Fpv->TargetArmLength = 0.0f;
	SpringArm_Fpv->bUsePawnControlRotation = true;
	SpringArm_Fpv->bInheritPitch = false;
	SpringArm_Fpv->bInheritYaw = false;
	SpringArm_Fpv->bInheritRoll = false;
	SpringArm_Fpv->bDoCollisionTest = false;
	
	Camera_Fpv->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	Camera_Fpv->bUsePawnControlRotation = true;

			// flash light
	FlashlightHolder = CreateDefaultSubobject<USceneComponent>(TEXT("FlashlightHolder"));
	FlashlightHolder->SetupAttachment(Camera_Fpv);
	FlashlightHolder->SetRelativeLocation(FVector(40.0f, -30.0f, -20.0f));
	FlashlightHolder->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

		// shoulder
	SpringArm_Shoulder = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm_Shoulder"));
	SpringArm_Shoulder->SetupAttachment(RootComponent);
	Camera_Shoulder = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera_Shoulder"));
	Camera_Shoulder->SetupAttachment(SpringArm_Shoulder, USpringArmComponent::SocketName);

	SpringArm_Shoulder->TargetArmLength = 500.0f;
	SpringArm_Shoulder->bUsePawnControlRotation = true;
	SpringArm_Shoulder->bInheritPitch = true;
	SpringArm_Shoulder->bInheritYaw = true;
	SpringArm_Shoulder->bInheritRoll = true;
	SpringArm_Shoulder->bDoCollisionTest = true;
	Camera_Shoulder->bUsePawnControlRotation = false;

		// quarter
	SpringArm_Quarter = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm_Quarter"));
	SpringArm_Quarter->SetupAttachment(RootComponent);
	Camera_Quarter = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera_Quarter"));
	Camera_Quarter->SetupAttachment(SpringArm_Quarter, USpringArmComponent::SocketName);

	SpringArm_Quarter->SetRelativeRotation(FRotator(0.0f, -30.0f, 0.0f));
	SpringArm_Quarter->TargetArmLength = 800.0f;
	SpringArm_Quarter->bUsePawnControlRotation = true;
	SpringArm_Quarter->bInheritPitch = false;
	SpringArm_Quarter->bInheritYaw = false;
	SpringArm_Quarter->bInheritRoll = false;
	SpringArm_Quarter->bDoCollisionTest = false;
	Camera_Quarter->bUsePawnControlRotation = false;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_ChangeControl.IA_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

		// view
			// FPV
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionFpvMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_FirstPersonViewMove.IA_FirstPersonViewMove'"));
	if (nullptr != InputActionFpvMoveRef.Object)
	{
		FirstPersonViewMoveAction = InputActionFpvMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionFpvLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_FirstPersonViewLook.IA_FirstPersonViewLook'"));
	if (nullptr != InputActionFpvLookRef.Object)
	{
		FirstPersonViewLookAction = InputActionFpvLookRef.Object;
	}
			// Shoulder
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_ShoulderViewMove.IA_ShoulderViewMove'"));
	if (nullptr != InputActionShMoveRef.Object)
	{
		ShoulderViewMoveAction = InputActionShMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_ShoulderViewLook.IA_ShoulderViewLook'"));
	if (nullptr != InputActionShLookRef.Object)
	{
		ShoulderViewLookAction = InputActionShLookRef.Object;
	}
			// Quarter
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQaMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_QuarterViewMove.IA_QuarterViewMove'"));
	if (nullptr != InputActionQaMoveRef.Object)
	{
		QuarterViewMoveAction = InputActionQaMoveRef.Object;
	}

		// view details data asset
	static ConstructorHelpers::FObjectFinder<UHRCharacterControlData> FirstDataRef(TEXT("/Script/UE5_HorrorGame.HRCharacterControlData'/Game/CharacterControl/HRC_FirstPersonView.HRC_FirstPersonView'"));
	if (FirstDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::First, FirstDataRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UHRCharacterControlData> ShoulderDataRef(TEXT("/Script/UE5_HorrorGame.HRCharacterControlData'/Game/CharacterControl/HRC_ShoulderView.HRC_ShoulderView'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UHRCharacterControlData> QuarterDataRef(TEXT("/Script/UE5_HorrorGame.HRCharacterControlData'/Game/CharacterControl/HRC_QuarterView.HRC_QuarterView'"));
	if (QuarterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quarter, QuarterDataRef.Object);
	}

		// interact
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionInteractRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Interact.IA_Interact'"));
	if (nullptr != InputActionInteractRef.Object)
	{
		InteractAction = InputActionInteractRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionToggleInventoryRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_ToggleInventory.IA_ToggleInventory'"));
	if (nullptr != InputActionToggleInventoryRef.Object)
	{
		ToggleInventoryAction = InputActionToggleInventoryRef.Object;
	}
		
		// flashlight
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionFlashLightRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_FlashLight.IA_FlashLight'"));
	if (nullptr != InputActionFlashLightRef.Object)
	{
		ToggleFlashlightAction = InputActionFlashLightRef.Object;
	}

	// Stat Component
	Stat = CreateDefaultSubobject<UHRCharacterStatComponent>(TEXT("Stat"));

	// Widget Component
	HpBar = CreateDefaultSubobject<UHRWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}

	// Inventory Component
		InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	// Flashlight
		FlashlightAttachSocketName = FName("Hand_Left_Socket_for_FlashLight");	// socket name
		hasFlashLight = false;

}

void AHRCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// set first to fpv
		APlayerController* PlayerController = CastChecked<APlayerController>(GetController());

		PlayerController->SetViewTarget(Camera_Fpv->GetOwner());
		Camera_Fpv->SetActive(true);
		Camera_Shoulder->SetActive(false);
		Camera_Quarter->SetActive(false);

		SetCharacterControl(CurrCharacterControlType);

		if (InventoryWidgetClass)
		{
			InventoryWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass);

			if (InventoryWidgetInstance)
			{
				// ������ ������ ���� �߰� ���� (���� ����)
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Inventory widget creation failure!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory widget class doesn't fixed in editor!"));
		}

}

void AHRCharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// hp�� 0�Ǹ� �װ��ϴ� ��������Ʈ, ������������, setdead�ȿ��� hpbar->sethiddeningame(true);
	// Stat->OnHpZero.AddUObject(this, &AHRCharacterPlayer::SetDead);
}

// IMC : mapping input to specipic action
// SetupPlayerInputComponent : bind trigger to specific function 
// FInputActionValue : automatically processed when the player possess the pawn
void AHRCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AHRCharacterPlayer::ChangeCharacterControl);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	// using customized func
		// view
	EnhancedInputComponent->BindAction(FirstPersonViewMoveAction, ETriggerEvent::Triggered, this, &AHRCharacterPlayer::FirstPersonViewMove);
	EnhancedInputComponent->BindAction(FirstPersonViewLookAction, ETriggerEvent::Triggered, this, &AHRCharacterPlayer::FirstPersonViewLook);

	EnhancedInputComponent->BindAction(ShoulderViewMoveAction, ETriggerEvent::Triggered, this, &AHRCharacterPlayer::ShoulderViewMove);
	EnhancedInputComponent->BindAction(ShoulderViewLookAction, ETriggerEvent::Triggered, this, &AHRCharacterPlayer::ShoulderViewLook);

	EnhancedInputComponent->BindAction(QuarterViewMoveAction, ETriggerEvent::Triggered, this, &AHRCharacterPlayer::QuarterViewMove);

		// interact
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AHRCharacterPlayer::Interact);
	EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Triggered, this, &AHRCharacterPlayer::ToggleInventory);

		// flashlight
	//EnhancedInputComponent->BindAction(ToggleFlashlightAction, ETriggerEvent::Triggered, this, &AHRCharacterPlayer::ToggleFlashLight);

	if (ToggleFlashlightAction) // ToggleFlashlightAction�� ��ȿ���� Ȯ��
	{
		EnhancedInputComponent->BindAction(ToggleFlashlightAction, ETriggerEvent::Triggered, this, &AHRCharacterPlayer::ToggleFlashLight);
		UE_LOG(LogTemp, Warning, TEXT("ToggleFlashlightAction BOUND successfully.")); // ���ε� ���� �α� �߰�
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ToggleFlashlightAction is NULLPTR. Cannot bind.")); // ���ε� ���� �α� �߰�
	}
}

//void AHRCharacterPlayer::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//
//}

//// debug
//FString EnumToString(ECharacterControlType EnumValue)
//{
//	UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECharacterControlType"), true);
//	if (!EnumPtr) return FString("Invalid");
//
//	return EnumPtr->GetNameStringByIndex((int32)EnumValue);
//}

void AHRCharacterPlayer::ChangeCharacterControl()
{
	if (CurrCharacterControlType == ECharacterControlType::First)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quarter);
	}
	else if (CurrCharacterControlType == ECharacterControlType::Quarter)
	{
		SetCharacterControl(ECharacterControlType::First);
	}

	// debug
	//UE_LOG(LogTemp, Warning, TEXT("%s\n"), *EnumToString(CurrCharacterControlType));
	//UE_LOG(LogTemp, Warning, TEXT("Arm Length : %f\n"), CameraBoom->TargetArmLength);
	//UE_LOG(LogTemp, Warning, TEXT("Controller Rotation: %s"), *GetControlRotation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("SpringArm Rotation : %s\n"), *CameraBoom->GetRelativeRotation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("SpringArm Location : %s\n"), *CameraBoom->GetComponentLocation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Camera Location : %s\n"), *FollowCamera->GetComponentLocation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Camera Rotation : %s\n"), *FollowCamera->GetComponentRotation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("-------------------------\n"));

}

void AHRCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UHRCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());

	// Bring local(client) player's subsystem(which manages enhanced-input on a local player unit)
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();

		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);		// 0 : priority

			//Subsystem->RemoveMappingContext(DefaultMappingContext);	// modifiable during runtime : remove default IMC -> add another IMC
		}
	}

	if (NewCharacterControlType == ECharacterControlType::First)
	{
		//PlayerController->SetViewTargetWithBlend(Camera_Shoulder->GetOwner(), 0.5f);

		Camera_Fpv->SetActive(true);
		Camera_Shoulder->SetActive(false);
		Camera_Quarter->SetActive(false);

	}
	else if (NewCharacterControlType == ECharacterControlType::Shoulder)
	{
		//PlayerController->SetViewTargetWithBlend(Camera_Quarter->GetOwner(), 0.5f);

		Camera_Fpv->SetActive(false);
		Camera_Shoulder->SetActive(true);
		Camera_Quarter->SetActive(false);
	}
	else if (NewCharacterControlType == ECharacterControlType::Quarter)
	{
		//PlayerController->SetViewTargetWithBlend(Camera_Fpv->GetOwner(), 0.5f);

		Camera_Fpv->SetActive(false);
		Camera_Shoulder->SetActive(false);
		Camera_Quarter->SetActive(true);
	}

	CurrCharacterControlType = NewCharacterControlType;
}

void AHRCharacterPlayer::SetCharacterControlData(const UHRCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;

	// Mesh
	GetMesh()->SetOwnerNoSee(CharacterControlData->bSetOwnerNoSee);
}

// FInputActionValue : struct that containing various input info ( refreshing every frame )
void AHRCharacterPlayer::FirstPersonViewMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();

	// direction vector
	// https://velog.io/@yoo06/UE5-Enhanced-Input-%EC%95%8C%EC%95%84%EB%B3%B4%EA%B8%B0
	const FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);

}
void AHRCharacterPlayer::FirstPersonViewLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AHRCharacterPlayer::ShoulderViewMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);

}
void AHRCharacterPlayer::ShoulderViewLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AHRCharacterPlayer::QuarterViewMove(const FInputActionValue& Value)
{
	// �����ڵ�, ������ �����ΰ�ó�� �ൿ
	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
	
	//// ���1, �޽� ȸ�� �����Լ� �ʿ�
	//FVector2D MovementVector = Value.Get<FVector2D>();

	//	// �Է� ���� ũ�� ���� (�밢�� �̵� �ӵ� ����)
	//if (MovementVector.SquaredLength() > 1.0f)
	//{
	//	MovementVector.Normalize();
	//}

	//	// ĳ���Ͱ� �ٶ󺸴� ������ �������� �̵� ���� ���
	//FRotator CameraRotation = Camera_Quarter->GetComponentRotation(); // ���ͺ� ī�޶��� ȸ��
	//FRotator YawRotation(0, CameraRotation.Yaw, 0); // Yaw ȸ���� ����
	//FVector MoveDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X) * MovementVector.X + FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y) * MovementVector.Y; // forward, right vector�� �̿��Ͽ� ������ ����.

	//AddMovementInput(MoveDirection, 1.0f);

	//// �̵� ������ ������ �޽� ȸ�� ���� (90�� ����)
	//if (!MoveDirection.IsNearlyZero())
	//{
	//	FRotator TargetRotation = MoveDirection.Rotation();
	//	TargetRotation.Yaw -= 90.0f; // �⺻ ȸ���� ����

	//	GetMesh()->SetWorldRotation(TargetRotation);
	//}
}

//// ���2, fpv�� ȸ���� �״�� �����Ǿ� ���� ����Ŭ���� ������ ����
//void AHRCharacterPlayer::QuarterViewMove(const FInputActionValue& Value)
//{
//	FVector2D MovementVector = Value.Get<FVector2D>();
//
//	// �Է� ���� ũ�� ���� (�밢�� �̵� �ӵ� ����)
//	if (MovementVector.SquaredLength() > 1.0f)
//	{
//		MovementVector.Normalize();
//	}
//
//	// ���ͺ� ī�޶��� ȸ�� �� ��������
//	FRotator CameraRotation = Camera_Quarter->GetComponentRotation();
//	FRotator YawRotation(0, CameraRotation.Yaw, 0);
//
//	// ī�޶� Yaw ȸ�� ���� �������� �̵� ���� ���� ���
//	FVector MoveDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X) * MovementVector.X + FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y) * MovementVector.Y;
//
//	// �̵� �������� ĳ���� �̵�
//	AddMovementInput(MoveDirection, 1.0f);
//
//
//	// �̵� ������ ���� ���� �޽� ȸ�� ����
//	if (!MoveDirection.IsNearlyZero())
//	{
//		// �̵� ���� ���� ���
//		FRotator TargetRotation = MoveDirection.Rotation();
//
//		// �޽� ȸ�� �� 90�� ������ ���� (�޽��� �ʱ� ���⿡ ���� ����)
//		TargetRotation.Yaw -= 90.0f;
//
//		// �ε巯�� ȸ���� ���� RInterp To ��� (������)
//		FRotator CurrentRotation = GetMesh()->GetComponentRotation();
//		FRotator InterpolatedRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 10.0f); // 10.0f�� ���� �ӵ�
//
//		GetMesh()->SetWorldRotation(InterpolatedRotation);
//	}
//}

void AHRCharacterPlayer::Interact()
{
	// linetrace
	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (!playerController) return;

	FVector start;
	FVector end;
	FHitResult hitResult;

	FVector cameraLocation;
	FRotator cameraRotation;
	playerController->GetPlayerViewPoint(cameraLocation, cameraRotation);

	start = cameraLocation;
	end = start + (cameraRotation.Vector() * 1000.0f); // modifiable line range

	// collision query decision struct
	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(this); // ignore itself (actor)

	// shoot trace
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		hitResult,
		start,
		end,
		ECC_Visibility,		// collision channel
		traceParams			// additional utility
	);

	// debug line
	FColor lineColor = bHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), start, end, lineColor, false, 1.0f, 0, 0.5f);

	if (bHit)
	{
		AActor* HitActor = hitResult.GetActor();
		if (HitActor)
		{
			// Item
			IHRItemInterface* Pickable = Cast<IHRItemInterface>(HitActor);
			if (Pickable)
			{
				if (Pickable->IsPickable())
				{
					AddItemToInventory(HitActor);

					Pickable->OnPickedUp(this);	// what should happen after picked up

					// debug
					UE_LOG(LogTemp, Warning, TEXT("Item picked up : %s"), *HitActor->GetName());
				}
			}
			else // not an item
			{
				IHRInteractableActorInterface* Interactable = Cast<IHRInteractableActorInterface>(HitActor);
				if (Interactable)
				{
					if (Interactable->IsInteractable())
					{
						// couldn't use like 'Interactable->interact();'
						// cuz its BlueprintNativeEvent
						IHRInteractableActorInterface::Execute_BP_Interact(HitActor, this);
					}
				}
			}
		}
	}
}

void AHRCharacterPlayer::AddItemToInventory(AActor* InItem)
{
	AHRItemBase* Item = Cast<AHRItemBase>(InItem);
	if (Item)
	{
		InventoryComponent->AddItem(Item);

		// debug
		UE_LOG(LogTemp, Warning, TEXT("Item added to inventory: %s"), *Item->GetName());

		if (InventoryWidgetInstance)
		{
			// �������Ʈ�� �ִ� UpdateInventoryUI �Լ� ȣ��
			FOutputDeviceNull n;
			InventoryWidgetInstance->CallFunctionByNameWithArguments
			(TEXT("UpdateInventoryUI"), n, nullptr, true);
		}
	}
}

void AHRCharacterPlayer::ToggleInventory()
{
	if (InventoryWidgetInstance)
	{
		FOutputDeviceNull n;

		InventoryWidgetInstance->CallFunctionByNameWithArguments(TEXT("ToggleInventory"), n, nullptr, true);
	}
}

void AHRCharacterPlayer::AttachFlashlight(AHRItemBase* FlashlightToAttach)
{
	if (FlashlightToAttach && FlashlightHolder) // FlashlightHolder ���
	{
		EquippedFlashlight = FlashlightToAttach;
		EquippedFlashlight->AttachToComponent(
			FlashlightHolder, // FlashlightHolder�� ����
			FAttachmentTransformRules::SnapToTargetIncludingScale // ��ġ/ȸ���� Holder�� ����
		);
		EquippedFlashlight->SetOwner(this);

		// Holder�� �ٿ����Ƿ�, ������ ��ü�� ��� ��ġ/ȸ���� (0,0,0)���� ���� (���� ����)
		EquippedFlashlight->SetActorRelativeLocation(FVector::ZeroVector);
		EquippedFlashlight->SetActorRelativeRotation(FRotator::ZeroRotator);

		UE_LOG(LogTemp, Log, TEXT("Flashlight attached to FlashlightHolder: %s"), *EquippedFlashlight->GetName());

		EquippedFlashlight->SetActorHiddenInGame(false);
		// ... (�޽� ���ü� ó��) ...
	}
}

void AHRCharacterPlayer::ToggleFlashLight()
{
	UE_LOG(LogTemp, Error, TEXT("TOGGLE FLASHLIGHT FUNCTION ENTERED!"));

	if (IsValid(EquippedFlashlight))
	{
		if (EquippedFlashlight->GetClass()->ImplementsInterface(UHRToggleItemInterface::StaticClass()))
		{
			IHRToggleItemInterface::Execute_BP_ToggleItem(EquippedFlashlight);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ToggleFlashLight: EquippedFlashlight (%s) DOES NOT IMPLEMENT UHRToggleItemInterface."), *EquippedFlashlight->GetName());
		}
	}
	else
	{
	UE_LOG(LogTemp, Error, TEXT("ToggleFlashLight: EquippedFlashlight is not VALID"));
	}
}

void AHRCharacterPlayer::SetupCharacterWidget(UHRUserWidget* InUserWidget)
{
	UHRHpBarWidget* HpBarWidget = Cast<UHRHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetMaxHp());
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());

		Stat->OnHpChanged.AddUObject(HpBarWidget, &UHRHpBarWidget::UpdateHpBar);
	}
}