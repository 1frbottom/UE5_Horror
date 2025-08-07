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
	FText ItemName;		// when the actor is placed
						// couldn't make auto wrap with no space, so use space to newline

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	UTexture2D* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	FText ItemDescription;

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

	// HRItemInterface
	UFUNCTION(BlueprintCallable, Category = "HRItemInterface")
	virtual void OnPickedUp(AHRCharacterPlayer* character) override;
	virtual bool IsPickable() const override { return bIsInteractable; }
	
		// for being seen in the BP
	UFUNCTION(BlueprintImplementableEvent, Category = "HRItemInterface")
	void BP_OnPickedUp(AHRCharacterPlayer* character);

// AHRInteractableActorBase
public:
	virtual FText GetInteractionText_Implementation() override;

};
