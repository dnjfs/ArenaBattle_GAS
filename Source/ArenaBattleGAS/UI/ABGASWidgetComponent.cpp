// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABGASWidgetComponent.h"
#include "UI/ABGASUserWidget.h"

void UABGASWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UABGASUserWidget* GASUserWidget = Cast<UABGASUserWidget>(GetWidget());
	if (GASUserWidget)
	{
		// 위젯 컴포넌트를 가지고 있는 액터를 넘겨줌
		GASUserWidget->SetAbilitySystemComponent(GetOwner());
	}
}