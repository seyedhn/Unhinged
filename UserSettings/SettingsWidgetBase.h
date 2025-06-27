// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/OurCommonActivatableWidgetBase.h"
#include "SettingsPDA.h"
#include "SettingsWidgetBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResetToDefaultDelegate, bool, bIsDefault);

UCLASS(Abstract)
class THEGAME_API USettingsWidgetBase : public UOurCommonActivatableWidgetBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta=(ReturnDisplayName = "IsDefaultValue"))
	bool LoadValue();

	UPROPERTY(BlueprintReadOnly, meta = (ExposeonSpawn = true))
	FInstancedStruct SettingProperties;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FResetToDefaultDelegate OnValueUpdated;

};