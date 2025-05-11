// Fill out your copyright notice in the Description page of Project Settings.


#include "Miscellaneous/InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UInventoryComponent::AddItem(AActor* Item)
{
    if (Item)
    {
        Inventory.Add(Item);

        UE_LOG(LogTemp, Log, TEXT("Item added: %s"), *Item->GetName());
    }
}

void UInventoryComponent::RemoveItem(AActor* Item)
{
    if (Item && Inventory.Contains(Item))
    {
        Inventory.Remove(Item);

        UE_LOG(LogTemp, Log, TEXT("Item removed: %s"), *Item->GetName());
    }
}

const TArray<AActor*>& UInventoryComponent::GetItems() const
{

    return Inventory;
}