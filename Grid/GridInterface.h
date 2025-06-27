// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridInterface.generated.h"



UINTERFACE(MinimalAPI)
class UGridInterface : public UInterface
{
	GENERATED_BODY()
};

class THEGAME_API IGridInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Grid)
	void OnPowerOutage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Grid)
	void OnPowerRestore();
};