// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HRItemBase.h"

// Sets default values
AHRItemBase::AHRItemBase()
{

}

void AHRItemBase::OnPickedUp()
{
	SetActorEnableCollision(false);		// collision
	SetActorHiddenInGame(true);			// visibility

	// implement destroying actor at child
}


