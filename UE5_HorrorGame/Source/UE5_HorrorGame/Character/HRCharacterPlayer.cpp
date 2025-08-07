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

//#include "Components/GridPanel.h"
//#include "Components/CanvasPanel.h"
//#include "Components/Image.h"
//#include "Components/TextBlock.h"
//#include "UMG.h"	// ugridslot

#include "Components/TimelineComponent.h"	// FOnTimelineFloat
#include "Misc/OutputDeviceNull.h"	// FOutputDeviceNull

#include "HRCharacterControlData.h"

#include "CharacterStat/HRCharacterStatComponent.h"
#include "UI/HRWidgetComponent.h"
#include "UI/HRHpBarWidget.h"

#include "Player/HRUIManagerComponent.h"

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

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->CrouchedHalfHeight = 50.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100.0f;


	WalkSpeed = 200.0f;
	SprintSpeed = 300.0f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn_1"));

	// Mesh ( test )
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

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

	// UI Actor Component
	UIManager = CreateDefaultSubobject<UHRUIManagerComponent>(TEXT("UIManager"));

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

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionCrouchRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Crouch.IA_Crouch'"));
	if (nullptr != InputActionCrouchRef.Object)
	{
		CrouchAction = InputActionCrouchRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSprintRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Sprint.IA_Sprint'"));
	if (nullptr != InputActionSprintRef.Object)
	{
		SprintAction = InputActionSprintRef.Object;
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
		
		// ToggleFlashlight
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionToggleFlashLightRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_ToggleFlashLight.IA_ToggleFlashLight'"));
	if (nullptr != InputActionToggleFlashLightRef.Object)
	{
		ToggleFlashlightAction = InputActionToggleFlashLightRef.Object;
	}
		
		// ToggleMap
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionToggleMapRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_ToggleMap.IA_ToggleMap'"));
	if (nullptr != InputActionToggleMapRef.Object)
	{
		ToggleMapAction = InputActionToggleMapRef.Object;
	}

		// CycleMap
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionCycleMapRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_CycleMap.IA_CycleMap'"));
	if (nullptr != InputActionCycleMapRef.Object)
	{
		CycleMapAction = InputActionCycleMapRef.Object;
	}

	// Stat Component
	Stat = CreateDefaultSubobject<UHRCharacterStatComponent>(TEXT("Stat"));

	// Widget Component
	//HpBar = CreateDefaultSubobject<UHRWidgetComponent>(TEXT("Widget"));
	//HpBar->SetupAttachment(GetMesh());
	//HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));

	//static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UI/WBP_HpBar.WBP_HpBar_C"));
	//if (HpBarWidgetRef.Class)
	//{
	//	HpBar->SetWidgetClass(HpBarWidgetRef.Class);
	//	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	//	HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
	//	HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//}

	// Inventory Component
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	// Flashlight
	FlashlightAttachSocketName = FName("Hand_Left_Socket_for_FlashLight");	// socket name
	hasFlashLight = false;

	// Map
	CurrentMapType = EMapType::EMT_None;
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
				// 생성된 위젯에 대한 추가 설정 (선택 사항)
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

	// very first, start checking collision with AHRInteractableActorBase 

		TArray<AActor*> InitialOverlappingActors;
		GetOverlappingActors(InitialOverlappingActors, AHRInteractableActorBase::StaticClass());

		if (InitialOverlappingActors.Num() > 0)
		{
			StartInteractionTrace();
		}

	// crouch timeline
		CrouchTimeline = NewObject<UTimelineComponent>(this, FName("CrouchTimeline"));
		CrouchTimeline->RegisterComponent();

		FOnTimelineFloat UpdateFunction;
		UpdateFunction.BindUFunction(this, FName("UpdateCrouch"));
		if (CrouchCurve)
		{
			CrouchTimeline->AddInterpFloat(CrouchCurve, UpdateFunction);
		}

		FOnTimelineEvent OnTimelineFinished;
		OnTimelineFinished.BindUFunction(this, FName("OnCrouchTimelineFinished"));
		CrouchTimeline->SetTimelineFinishedFunc(OnTimelineFinished);
		CrouchTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
}

void AHRCharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// hp가 0되면 죽게하는 델리게이트, 아직구현안함, setdead안에는 hpbar->sethiddeningame(true);
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

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AHRCharacterPlayer::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AHRCharacterPlayer::StartCrouch);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AHRCharacterPlayer::StopCrouch);

	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AHRCharacterPlayer::StartSprint);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHRCharacterPlayer::StopSprint);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &AHRCharacterPlayer::StopSprint);


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
	if (ToggleFlashlightAction) // ToggleFlashlightAction이 유효한지 확인
	{
		EnhancedInputComponent->BindAction(ToggleFlashlightAction, ETriggerEvent::Triggered, this, &AHRCharacterPlayer::ToggleFlashLight);
		UE_LOG(LogTemp, Warning, TEXT("ToggleFlashlightAction BOUND successfully.")); // 바인딩 성공 로그 추가
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ToggleFlashlightAction is NULLPTR. Cannot bind.")); // 바인딩 실패 로그 추가
	}

		// map
	EnhancedInputComponent->BindAction(ToggleMapAction, ETriggerEvent::Triggered, this, &AHRCharacterPlayer::ToggleMap);
	EnhancedInputComponent->BindAction(CycleMapAction, ETriggerEvent::Triggered, this, &AHRCharacterPlayer::CycleMap);

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

void AHRCharacterPlayer::Jump()
{
	if (UIManager && UIManager->IsUIActive())
		return;

	Super::Jump();

	// could modify jump velocity or more
}

void AHRCharacterPlayer::StartCrouch()
{
	if (UIManager && UIManager->IsUIActive())
		return;

	if (CrouchTimeline)
	{
		CrouchTimeline->PlayFromStart();
	}
}
void AHRCharacterPlayer::StopCrouch()
{
	if (UIManager && UIManager->IsUIActive())
		return;

	//UnCrouch();
	if (CrouchTimeline)
	{
		CrouchTimeline->ReverseFromEnd();
	}
}

void AHRCharacterPlayer::UpdateCrouch(float Value)
{
	const float StandingHeight = GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	const float CrouchingHeight = GetCharacterMovement()->CrouchedHalfHeight;
	const float NewHeight = FMath::Lerp(StandingHeight, CrouchingHeight, Value);
	GetCapsuleComponent()->SetCapsuleHalfHeight(NewHeight);
}

void AHRCharacterPlayer::OnCrouchTimelineFinished()
{
	if (CrouchTimeline)
	{
		if (CrouchTimeline->GetPlaybackPosition() == 0.0f)
		{
			UnCrouch();
		}
		else if (CrouchTimeline->GetPlaybackPosition() >= CrouchTimeline->GetTimelineLength())
		{
			Crouch();
		}
	}
}

void AHRCharacterPlayer::StartSprint()
{
	if (UIManager && UIManager->IsUIActive())
		return;

	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	UE_LOG(LogTemp, Warning, TEXT("StartSprint - MaxWalkSpeed actually set to: %f"), GetCharacterMovement()->MaxWalkSpeed);
}
void AHRCharacterPlayer::StopSprint()
{
	if (UIManager && UIManager->IsUIActive())
		return;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	UE_LOG(LogTemp, Warning, TEXT("StopSprint - MaxWalkSpeed actually set to: %f"), GetCharacterMovement()->MaxWalkSpeed);

}

// FInputActionValue : struct that containing various input info ( refreshing every frame )
void AHRCharacterPlayer::FirstPersonViewMove(const FInputActionValue& Value)
{
	if (UIManager && UIManager->IsUIActive())
		return;

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
	if (UIManager && UIManager->IsUIActive())
		return;

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
	// 원본코드, 왼쪽이 정면인것처럼 행동
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
	
	//// 대안1, 메쉬 회전 보간함수 필요
	//FVector2D MovementVector = Value.Get<FVector2D>();

	//	// 입력 벡터 크기 조절 (대각선 이동 속도 보정)
	//if (MovementVector.SquaredLength() > 1.0f)
	//{
	//	MovementVector.Normalize();
	//}

	//	// 캐릭터가 바라보는 방향을 기준으로 이동 방향 계산
	//FRotator CameraRotation = Camera_Quarter->GetComponentRotation(); // 쿼터뷰 카메라의 회전
	//FRotator YawRotation(0, CameraRotation.Yaw, 0); // Yaw 회전만 추출
	//FVector MoveDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X) * MovementVector.X + FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y) * MovementVector.Y; // forward, right vector를 이용하여 방향을 구함.

	//AddMovementInput(MoveDirection, 1.0f);

	//// 이동 방향이 있으면 메쉬 회전 적용 (90도 보정)
	//if (!MoveDirection.IsNearlyZero())
	//{
	//	FRotator TargetRotation = MoveDirection.Rotation();
	//	TargetRotation.Yaw -= 90.0f; // 기본 회전값 보정

	//	GetMesh()->SetWorldRotation(TargetRotation);
	//}
}

//// 대안2, fpv의 회전이 그대로 보전되어 다음 사이클에도 나오는 버그
//void AHRCharacterPlayer::QuarterViewMove(const FInputActionValue& Value)
//{
//	FVector2D MovementVector = Value.Get<FVector2D>();
//
//	// 입력 벡터 크기 조절 (대각선 이동 속도 보정)
//	if (MovementVector.SquaredLength() > 1.0f)
//	{
//		MovementVector.Normalize();
//	}
//
//	// 쿼터뷰 카메라의 회전 값 가져오기
//	FRotator CameraRotation = Camera_Quarter->GetComponentRotation();
//	FRotator YawRotation(0, CameraRotation.Yaw, 0);
//
//	// 카메라 Yaw 회전 값을 기준으로 이동 방향 벡터 계산
//	FVector MoveDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X) * MovementVector.X + FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y) * MovementVector.Y;
//
//	// 이동 방향으로 캐릭터 이동
//	AddMovementInput(MoveDirection, 1.0f);
//
//
//	// 이동 방향이 있을 때만 메쉬 회전 적용
//	if (!MoveDirection.IsNearlyZero())
//	{
//		// 이동 방향 각도 계산
//		FRotator TargetRotation = MoveDirection.Rotation();
//
//		// 메쉬 회전 시 90도 오프셋 보정 (메쉬의 초기 방향에 따라 조절)
//		TargetRotation.Yaw -= 90.0f;
//
//		// 부드러운 회전을 위해 RInterp To 사용 (선택적)
//		FRotator CurrentRotation = GetMesh()->GetComponentRotation();
//		FRotator InterpolatedRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 10.0f); // 10.0f은 보간 속도
//
//		GetMesh()->SetWorldRotation(InterpolatedRotation);
//	}
//}

// spinning off from 
void AHRCharacterPlayer::TraceInteractable()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;

	FVector start;
	FRotator rotation;
	PlayerController->GetPlayerViewPoint(start, rotation);

	FVector end = start + (rotation.Vector() * 1000.0f);
	FHitResult hitResult;
	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		hitResult,
		start,
		end,
		ECC_GameTraceChannel1, // ECC_Visibility,
		traceParams
	);

	// debug ~
	if (bHit)
	{
		// LineTrace에 맞은 액터와 컴포넌트의 이름을 로그로 출력
		UE_LOG(LogTemp, Warning, TEXT("LineTrace HIT --- Actor: [%s], Component: [%s]"),
			*hitResult.GetActor()->GetName(),
			*hitResult.GetComponent()->GetName());
	}
	else
	{
		// 아무것도 맞지 않았을 때 로그 출력
		UE_LOG(LogTemp, Warning, TEXT("LineTrace HIT --- NOTHING"));
	}
	// ~ debug

	AHRInteractableActorBase* FoundInteractable = nullptr;

	if (bHit)
	{
		AActor* hitActor = hitResult.GetActor();

		// should be bigger than around 100
		// cuz it calculates distance from each pivot
		const float maxInteractionDistance = 250.0f;	

		// check distance
		if (hitActor && FVector::Dist(GetActorLocation(), hitActor->GetActorLocation()) < maxInteractionDistance)
		{
			FoundInteractable = Cast<AHRInteractableActorBase>(hitActor);

			// debug
			const float distance = FVector::Dist(GetActorLocation(), hitActor->GetActorLocation());
			UE_LOG(LogTemp, Log, TEXT("1. Trace Hit: %s"), *hitActor->GetName());
			UE_LOG(LogTemp, Log, TEXT("2. Distance to Target: %f"), distance);
		}
	}

	if (FoundInteractable == nullptr)
	{
		// 이전에 포커스된 액터가 있었다면 포커스를 잃게 함
		if (IsValid(FocusedActor))
		{
			FocusedActor->OnFocusLost();
			FocusedActor = nullptr;
		}
	}
	else
	{
		if (FoundInteractable != FocusedActor)
		{
			if (FocusedActor)
			{
				FocusedActor->OnFocusLost();
			}

			FoundInteractable->OnFocusGained();
			FocusedActor = FoundInteractable;
		}
	}
}

void AHRCharacterPlayer::Interact()
{
	//// linetrace
	//APlayerController* playerController = Cast<APlayerController>(GetController());
	//if (!playerController) return;

	//FVector start;
	//FVector end;
	//FHitResult hitResult;

	//FVector cameraLocation;
	//FRotator cameraRotation;
	//playerController->GetPlayerViewPoint(cameraLocation, cameraRotation);

	//start = cameraLocation;
	//end = start + (cameraRotation.Vector() * 1000.0f); // modifiable line range

	//// collision query decision struct
	//FCollisionQueryParams traceParams;
	//traceParams.AddIgnoredActor(this); // ignore itself (actor)

	//// shoot trace
	//bool bHit = GetWorld()->LineTraceSingleByChannel(
	//	hitResult,
	//	start,
	//	end,
	//	ECC_Visibility,		// collision channel
	//	traceParams			// additional utility
	//);

	//// debug line
	//FColor lineColor = bHit ? FColor::Green : FColor::Red;
	//DrawDebugLine(GetWorld(), start, end, lineColor, false, 1.0f, 0, 0.5f);

	//if (bHit)
	//{
	//	AActor* HitActor = hitResult.GetActor();
	//	if (HitActor)
	//	{
	//		// Item
	//		IHRItemInterface* Pickable = Cast<IHRItemInterface>(HitActor);
	//		if (Pickable)
	//		{
	//			if (Pickable->IsPickable())
	//			{
	//				AddItemToInventory(HitActor);

	//				Pickable->OnPickedUp(this);	// what should happen after picked up

	//				// debug
	//				UE_LOG(LogTemp, Warning, TEXT("Item picked up : %s"), *HitActor->GetName());
	//			}
	//		}
	//		else // not an item
	//		{
	//			IHRInteractableActorInterface* Interactable = Cast<IHRInteractableActorInterface>(HitActor);
	//			if (Interactable)
	//			{
	//				if (Interactable->IsInteractable())
	//				{
	//					// couldn't use like 'Interactable->interact();'
	//					// cuz its BlueprintNativeEvent
	//					IHRInteractableActorInterface::Execute_BP_Interact(HitActor, this);
	//				}
	//			}
	//		}
	//	}
	//}

	// only for debug
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		FVector start;
		FRotator rotation;
		PlayerController->GetPlayerViewPoint(start, rotation);

		FVector end = start + (rotation.Vector() * 1000.0f);
		FHitResult hitResult;
		FCollisionQueryParams traceParams;
		traceParams.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			hitResult,
			start,
			end,
			ECC_Visibility,
			traceParams
		);

		FColor lineColor = bHit ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), start, end, lineColor, false, 2.0f, 0, 1.0f);
	}

	//// Trace 로직이 모두 사라지고, 이 조건문만 남음
	//if (FocusedActor) // 타이머가 찾아놓은 액터를 바로 사용
	//{
	//	// Item인지 확인
	//	if (IHRItemInterface* Pickable = Cast<IHRItemInterface>(FocusedActor))
	//	{
	//		if (Pickable->IsPickable())
	//		{
	//			AddItemToInventory(FocusedActor);
	//			Pickable->OnPickedUp(this);
	//		}
	//	}
	//	// Item이 아니면 일반 Interactable인지 확인
	//	else if (IHRInteractableActorInterface* Interactable = Cast<IHRInteractableActorInterface>(FocusedActor))
	//	{
	//		if (Interactable->IsInteractable())
	//		{
	//			IHRInteractableActorInterface::Execute_BP_Interact(FocusedActor, this);
	//		}
	//	}
	//}

	if (FocusedActor)
	{
		if (FocusedActor->GetClass()->ImplementsInterface(UHRInteractableActorInterface::StaticClass()))
		{
			// 플레이어는 더 이상 아이템인지, 문인지 구별하지 않습니다.
			// 그냥 상호작용하라는 신호만 보냅니다.
			IHRInteractableActorInterface::Execute_BP_Interact(FocusedActor, this);
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
			// 블루프린트에 있는 UpdateInventoryUI 함수 호출
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

void AHRCharacterPlayer::RegisterInteractableActor(AHRInteractableActorBase* InteractableActor)
{
	OverlappedInteractableActors.Add(InteractableActor);

	if (OverlappedInteractableActors.Num() == 1)
	{
		StartInteractionTrace();
	}
}

void AHRCharacterPlayer::UnregisterInteractableActor(AHRInteractableActorBase* InteractableActor)
{
	OverlappedInteractableActors.Remove(InteractableActor);

	if (OverlappedInteractableActors.Num() == 0)
	{
		StopInteractionTrace();
	}
}

void AHRCharacterPlayer::StartInteractionTrace()
{
	GetWorld()->GetTimerManager().SetTimer(
		TraceTimerHandle,
		this,
		&AHRCharacterPlayer::TraceInteractable,
		0.01f,
		true);
}

void AHRCharacterPlayer::StopInteractionTrace()
{
	GetWorld()->GetTimerManager().ClearTimer(TraceTimerHandle);

	if (FocusedActor)
	{
		FocusedActor->OnFocusLost();
		FocusedActor = nullptr;
	}
}

void AHRCharacterPlayer::AttachFlashlight(AHRItemBase* FlashlightToAttach)
{
	if (FlashlightToAttach && FlashlightHolder) // FlashlightHolder 사용
	{
		EquippedFlashlight = FlashlightToAttach;
		EquippedFlashlight->AttachToComponent(
			FlashlightHolder, // FlashlightHolder에 부착
			FAttachmentTransformRules::SnapToTargetIncludingScale // 위치/회전을 Holder에 맞춤
		);
		EquippedFlashlight->SetOwner(this);

		// Holder에 붙였으므로, 손전등 자체의 상대 위치/회전은 (0,0,0)으로 리셋 (선택 사항)
		EquippedFlashlight->SetActorRelativeLocation(FVector::ZeroVector);
		EquippedFlashlight->SetActorRelativeRotation(FRotator::ZeroRotator);

		UE_LOG(LogTemp, Log, TEXT("Flashlight attached to FlashlightHolder: %s"), *EquippedFlashlight->GetName());

		EquippedFlashlight->SetActorHiddenInGame(false);
		// ... (메시 가시성 처리) ...
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

void AHRCharacterPlayer::AcquireMap(EMapType NewMapType)
{
	// OwnedMaps 목록(Set)에 새로운 지도를 추가합니다.
	OwnedMaps.Add(NewMapType);

	// 만약 플레이어가 이전에 아무 지도도 가지고 있지 않았다면 (처음 지도를 얻는 경우),
	// 현재 선택된 지도를 방금 얻은 지도로 설정해줍니다.
	if (CurrentMapType == EMapType::EMT_None)
	{
		CurrentMapType = NewMapType;
	}

	UE_LOG(LogTemp, Warning, TEXT("Map Acquired: %s"), *UEnum::GetValueAsString(NewMapType));

	// 만약 맵 위젯이 열려있는 상태에서 새 지도를 획득했다면, 즉시 업데이트합니다.
	if (MapWidgetInstance && MapWidgetInstance->IsInViewport())
	{
		FOutputDeviceNull n;
		MapWidgetInstance->CallFunctionByNameWithArguments(TEXT("UpdateMapImage"), n, nullptr, true);
	}
}

void AHRCharacterPlayer::CycleMap()
{
	// 소유한 지도가 하나도 없다면 함수를 즉시 종료합니다.
	if (OwnedMaps.Num() == 0)
	{
		CurrentMapType = EMapType::EMT_None;
		UE_LOG(LogTemp, Log, TEXT("Player owns no maps to cycle."));
		return;
	}

	// TSet을 TArray로 변환해야 순서대로 접근할 수 있습니다.
	TArray<EMapType> OwnedMapsArray = OwnedMaps.Array();
	// Enum 순서대로 정렬하여 일관된 순서를 보장합니다.
	OwnedMapsArray.Sort();

	// 현재 선택된 지도의 인덱스를 찾습니다.
	int32 CurrentIndex = OwnedMapsArray.Find(CurrentMapType);

	// 현재 선택된 맵이 없거나 목록에 없으면(INDEX_NONE), 그냥 첫 번째 소유 지도로 설정합니다.
	if (CurrentIndex == INDEX_NONE)
	{
		CurrentMapType = OwnedMapsArray[0];
	}
	else
	{
		// 다음 인덱스로 이동합니다.
		CurrentIndex++;
		// 만약 인덱스가 배열의 끝을 넘어가면, 다시 0번으로 (처음으로) 돌아갑니다.
		if (CurrentIndex >= OwnedMapsArray.Num())
		{
			CurrentIndex = 0;
		}
		CurrentMapType = OwnedMapsArray[CurrentIndex];
	}

	UE_LOG(LogTemp, Warning, TEXT("Current Map Set To: %s"), *UEnum::GetValueAsString(CurrentMapType));

	// 맵이 열려있다면 즉시 업데이트
	if (MapWidgetInstance && MapWidgetInstance->IsInViewport())
	{
		FOutputDeviceNull n;
		MapWidgetInstance->CallFunctionByNameWithArguments(TEXT("UpdateMapImage"), n, nullptr, true);
	}
}

void AHRCharacterPlayer::ToggleMap()
{
	APlayerController* PlayerController = GetController<APlayerController>();
	if (!PlayerController)
	{
		return;
	}

	if (!UIManager)
	{
		UE_LOG(LogTemp, Error, TEXT("UIManager component is not valid on this character"));
		return;
	}

	if (MapWidgetInstance && MapWidgetInstance->IsInViewport())
	{
		// [지도 끄기] 이 로직은 그대로 유지합니다.
		UIManager->SetInputModeGameCustom();

		MapWidgetInstance->RemoveFromParent();
		MapWidgetInstance = nullptr;
	}
	else
	{
		// [지도 켜기] 로직이 매우 간결해집니다.
		if (MapWidgetClass)
		{
			MapWidgetInstance = CreateWidget<UUserWidget>(PlayerController, MapWidgetClass);
			if (MapWidgetInstance)
			{
				MapWidgetInstance->AddToViewport(); // 생성하고 뷰포트에 추가하는 역할만 수행
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MapWidgetClass is not set in the Character Blueprint!"));
		}
	}
}

//void AHRCharacterPlayer::SetupCharacterWidget(UHRUserWidget* InUserWidget)
//{
//	UHRHpBarWidget* HpBarWidget = Cast<UHRHpBarWidget>(InUserWidget);
//	if (HpBarWidget)
//	{
//		HpBarWidget->SetMaxHp(Stat->GetMaxHp());
//		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
//
//		Stat->OnHpChanged.AddUObject(HpBarWidget, &UHRHpBarWidget::UpdateHpBar);
//	}
//}