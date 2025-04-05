// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_HORRORGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

    TArray<AActor*> Inventory;
public:	
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void AddItem(AActor* Item);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void RemoveItem(AActor* Item);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    const TArray<AActor*>& GetItems() const;
		
};
