// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Miscellaneous/InventoryComponent.h"
#include "Interface/HRCharacterWidgetInterface.h"
#include "HRCharacterPlayer.generated.h"

#define ECC_Interactable ECollisionChannel::ECC_GameTraceChannel1

class AHRInteractableActorBase;

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

UCLASS()
class UE5_HORRORGAME_API AHRCharacterPlayer : public ACharacter, public IHRCharacterWidgetInterface
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

// Mesh ( test )
protected:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* CharacterMesh;

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

// Input
// in this project, pawn controll the input system. Not the pc
// using enhanced input ( add module to build.cs )
	// view
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CrouchAction;
	void StartCrouch();
	void StopCrouch();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SprintAction;
	void StartSprint();
	void StopSprint();
	float WalkSpeed;
	float SprintSpeed;

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

	// interact
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractAction;
	void Interact();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ToggleInventoryAction;
	UFUNCTION(BlueprintCallable, Category = "UI") // 블루프린트에서 구현할 함수, BlueprintCallable로 변경
		void ToggleInventory();

	// flashlight
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ToggleFlashlightAction;
	void ToggleFlashLight();


// Stat
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UHRCharacterStatComponent> Stat;

//public:
//	UFUNCTION(BlueprintCallable, Category = "Stat")
//	float GetCurrentHp() const { return CurrentHp; };
//
//protected:
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
//	float CurrentHp;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
//	float MaxHp;
//
//	void SetHp(float NewHp) { CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp); };

// UI Widget
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UHRWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class UHRUserWidget* InUserWidget) override;

// Inventory
protected:
	// inventory actor component
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UInventoryComponent* InventoryComponent;

		void AddItemToInventory(AActor* Item);
public:
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

};
