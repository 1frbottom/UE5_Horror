// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Miscellaneous/InventoryComponent.h"
#include "Interface/HRCharacterWidgetInterface.h"
#include "UI/HRStaminaBarWidget.h"
#include "UI/HRHpEffectWidget.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

#include "HRCharacterPlayer.generated.h"

#define ECC_Interactable ECollisionChannel::ECC_GameTraceChannel1	// Interaction

class USpringArmComponent;
class UCameraComponent;
class USceneComponent;
class UInputAction;
class UHRCharacterControlData;
class UHRCharacterStatComponent;
class UHRWidgetComponent;
class AHRItemBase;
class AHRInteractableActorBase;
class UTimelineComponent;

UENUM()
enum class ECharacterControlType : uint8
{
	First,
	Shoulder,
	Quarter
};

UENUM(BlueprintType)
enum class EKeyType : uint8
{
	A,
	B,
	C,
	D,
	E
};

UENUM(BlueprintType)
enum class EMapType : uint8
{
	EMT_None UMETA(DisplayName = "No Map"),       // 지도 없음
	EMT_Floor1 UMETA(DisplayName = "1st Floor"),  // 1층
	EMT_Floor2 UMETA(DisplayName = "2nd Floor"),  // 2층

	EMT_MAX UMETA(Hidden) // Enum 개수 확인용 (항상 마지막에 위치)
};

UCLASS()
class UE5_HORRORGAME_API AHRCharacterPlayer : public ACharacter/*, public IHRCharacterWidgetInterface*/
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHRCharacterPlayer();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void PostInitializeComponents() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//virtual void Tick(float DeltaTime) override;

// Mesh
protected:

// Camera
protected:

		// fpv
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm_Fpv;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera_Fpv;
		// flash light
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class USceneComponent> FlashlightHolder;

		// shoulder
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm_Shoulder;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera_Shoulder;
	
		// quarter
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm_Quarter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera_Quarter;

// UI Actor Component
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UHRUIManagerComponent> UIManager;
public:
	UFUNCTION(BlueprintPure, Category = "UI")
	UHRUIManagerComponent* GetUIManager() { return UIManager; }

// Input
// in this project, pawn controll the input system. Not the pc
// using enhanced input ( add module to build.cs )
	// view
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> FirstPersonViewMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> FirstPersonViewLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderViewLookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderViewMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuarterViewMoveAction;

	void FirstPersonViewMove(const FInputActionValue& Value);
	void FirstPersonViewLook(const FInputActionValue& Value);
	void ShoulderViewMove(const FInputActionValue& Value);
	void ShoulderViewLook(const FInputActionValue& Value);
	void QuarterViewMove(const FInputActionValue& Value);	// no need Look Func for Quarter View

	ECharacterControlType CurrCharacterControlType;

		// view selection
protected:
	void SetCharacterControlData(const class UHRCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = "CharacterControl", Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UHRCharacterControlData*> CharacterControlManager;

	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);

	// move
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;
	virtual void Jump() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CrouchAction;
	void StartCrouch();
	void StopCrouch();

	UFUNCTION()
	void UpdateCrouch(float Value);
	UFUNCTION()
	void OnCrouchTimelineFinished();

	UPROPERTY()
	TObjectPtr<UTimelineComponent> CrouchTimeline;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> CrouchCurve;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SprintAction;
	void StartSprint();
	void StopSprint();
	float WalkSpeed;
	float SprintSpeed;

	// Interact
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractAction;
	void Interact();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ToggleInventoryAction;
	UFUNCTION(BlueprintCallable, Category = "UI") // 블루프린트에서 구현할 함수, BlueprintCallable로 변경
		void ToggleInventory();

	// ToggleFlashlight
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ToggleFlashlightAction;
	void ToggleFlashLight();

	// ToggleMap
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ToggleMapAction;
	void ToggleMap();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CycleMapAction;
	void CycleMap();

// Stat
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UHRCharacterStatComponent> Stat;

	// Hp
	virtual void SetDead();		// 예상 : 델리게이트 터지면 캐릭터 bp에서 카메라, 위젯 처리하도록

	UFUNCTION()
	void OnHpChanged(float CurrentHp);

	public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Stamina
	protected:
		float SprintCost_sec;
		float JumpStaminaCost;
		float StaminaRegenRate_sec;

		void UpdateStamina();
		void HideStaminaBar();

	private:
		FTimerHandle StaminaTimerHandle;
	
		UFUNCTION()
		void OnStaminaChanged(float CurrentStamina);

		FTimerHandle StaminaUIVisibilityTimerHandle;
		UPROPERTY(EditAnywhere, Category = "UI", meta = (AllowPrivateAccess = "true"))
		float StaminaUIVisibilityDelay = 1.0f;

// UI widget
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UHRStaminaBarWidget> StaminaBarWidgetClass;
	UPROPERTY()
	TObjectPtr<UHRStaminaBarWidget> StaminaBarWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UHRHpEffectWidget> HpEffectWidgetClass;
	UPROPERTY()
	TObjectPtr<UHRHpEffectWidget> HpEffectWidgetInstance;


// Inventory
protected:
	// inventory actor component
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UInventoryComponent* InventoryComponent;

public:
	void AddItemToInventory(AActor* Item);

	UFUNCTION(BlueprintCallable, Category = "Components")
	class UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	// ui instance
protected:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> InventoryWidgetClass;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		class UUserWidget* InventoryWidgetInstance;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> InventorySlotWidgetClass;

// HRInteractableActorBase
public:
	void RegisterInteractableActor(AHRInteractableActorBase* InteractableActor);
	void UnregisterInteractableActor(AHRInteractableActorBase* InteractableActor);
private:
	void StartInteractionTrace();
	void StopInteractionTrace();

	UPROPERTY()
	TObjectPtr<AHRInteractableActorBase> FocusedActor;
	FTimerHandle TraceTimerHandle;

	UPROPERTY()
	TSet<AHRInteractableActorBase*> OverlappedInteractableActors;

	void TraceInteractable();

// HRItemBase

	// key
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	TSet<EKeyType> KeyChain;
public:
	UFUNCTION(BlueprintCallable, Category = "Key")
	bool GetHasKey(EKeyType key) { return KeyChain.Contains(key); }
	UFUNCTION(BlueprintCallable, Category = "Key")
	void SetHasKey(EKeyType key)  { KeyChain.Add(key); }

	// flashlight
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	bool hasFlashLight;

	UPROPERTY(EditDefaultsOnly, Category = "Flashlight")
	FName FlashlightAttachSocketName;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Flashlight", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AHRItemBase> EquippedFlashlight;

public:
	UFUNCTION(BlueprintCallable, Category = "Flashlight")
	void AttachFlashlight(AHRItemBase* FlashlightToAttach);

	// 현재 손전등이 장착되어 있는지 확인하는 함수 (BlueprintPure로 만들면 BP에서 사용하기 편함)
	UFUNCTION(BlueprintPure, Category = "Flashlight")
	bool HasFlashlightEquipped() const { return hasFlashLight; }

	// map
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MapWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	class UUserWidget* MapWidgetInstance;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map")
	EMapType CurrentMapType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map")
	TSet<EMapType> OwnedMaps;

	UFUNCTION(BlueprintCallable, Category = "Map")
	void AcquireMap(EMapType NewMapType);

// AI
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* StimuliSource;


// Miscellaneous
	





};
