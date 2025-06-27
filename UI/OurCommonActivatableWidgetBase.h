// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"

#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/NavigationConfig.h"

#include "OurCommonActivatableWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class THEGAME_API UOurCommonActivatableWidgetBase : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	bool bRegisterEnhancedInput;


};