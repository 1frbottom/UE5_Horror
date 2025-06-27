// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Interface/HRItemInterface.h"
#include "Delegates/DelegateCombinations.h"
#include "InteractableActor/HRInteractableActorBase.h"
#include "HRItemBase.generated.h"

class AHRCharacterPlayer;

UENUM()
enum class EItemType : uint8
{
	Potion,
	Weapon,
	Key,
	Map,
	QuestItem,
	Util
};

UCLASS()
class UE5_HORRORGAME_API AHRItemBase : public AHRInteractableActorBase, public IHRItemInterface
{
	GENERATED_BODY()
	
public:	
	AHRItemBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	UTexture2D* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	bool bIsPickable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	int32 Quantity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//USphereComponent* CollisionComponent;

public:
	UFUNCTION(BlueprintPure, Category = "Item Info")
	FName GetItemID() const { return ItemID; }

	UFUNCTION(BlueprintPure, Category = "Item Info")
	EItemType GetItemType() const { return ItemType; }

	UFUNCTION(BlueprintPure, Category = "Item Info")
	FText GetItemName() const { return ItemName; }

	UFUNCTION(BlueprintPure, Category = "Item Info")
	UTexture2D* GetItemImage() const { return ItemImage; }

	UFUNCTION(BlueprintPure, Category = "Item Info")
	FText GetItemDescription() const { return ItemDescription; }

	UFUNCTION(BlueprintPure, Category = "Item Info")
	int32 GetQuantity() const { return Quantity; }

	UFUNCTION(BlueprintCallable, Category = "Item Info")
	void SetQuantity(int32 NewQuantity) { Quantity = NewQuantity; }

	//UFUNCTION(BlueprintPure, Category = "Item Info")
	//bool IsPickable() const { return bIsPickable; }

	// temporary disable item, maybe useful
	UFUNCTION(BlueprintCallable, Category = "Item Info")
	void SetIsPickable(bool bNewPickable) { bIsPickable = bNewPickable; }

	// HRItemInterface
	virtual bool IsPickable() const override { return bIsPickable; }
	virtual void OnPickedUp(AHRCharacterPlayer* character) override;

		// for being seen in the BP
	UFUNCTION(BlueprintImplementableEvent, Category = "HRItemInterface")
	void BP_OnPickedUp(AHRCharacterPlayer* character);

// AHRInteractableActorBase
public:
	virtual bool IsInteractable() const override;

	virtual FText GetInteractionText_Implementation() override;

};
