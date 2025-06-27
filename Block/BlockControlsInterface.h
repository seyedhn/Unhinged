// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockControlsInterface.generated.h"



UINTERFACE(MinimalAPI)
class UBlockControlsInterface : public UInterface
{
	GENERATED_BODY()
};

class THEGAME_API IBlockControlsInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = "Block Controls")
	void Movement(const float Direction, const FRotator& Rotation);

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = "Block Controls")
	void Steering(const float Direction, const FRotator& Rotation);

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = "Block Controls")
	void Activate();

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = "Block Controls")
	void Possess(const AActor* Cockpit, const int32 KeyInput);

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = "Block Controls")
	void GotOnCockpit(const bool GotOn);
};