// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombustionInterface.generated.h"



UINTERFACE(MinimalAPI)
class UCombustionInterface : public UInterface
{
	GENERATED_BODY()
};

class THEGAME_API ICombustionInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = "Combustion")
	void BurnStarted(const int32 Index);

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = "Combustion")
	void BurnLoop(const float MassRemainingRatio, const int32 Index);

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = "Combustion")
	void BurnCompleted(const int32 Index);

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = "Combustion")
	void BurnExtinguished(const int32 Index);
};