// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HRWidgetComponent.h"
#include "HRUserWidget.h"


void UHRWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UHRUserWidget* HRUserWidget = Cast<UHRUserWidget>(GetWidget());

	if (HRUserWidget)
	{
		HRUserWidget->SetOwningActor(GetOwner());
	}

}
