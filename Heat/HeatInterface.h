// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeatInterface.generated.h"



UINTERFACE(MinimalAPI)
class UHeatInterface : public UInterface
{
	GENERATED_BODY()
};

class THEGAME_API IHeatInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = "Heat")
	void UpdateHeat(const float Temperature);

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = "Heat")
	void ProcessHeat();
};