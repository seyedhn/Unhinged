// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrabInterface.generated.h"



UINTERFACE(MinimalAPI)
class UGrabInterface : public UInterface
{
	GENERATED_BODY()
};

class THEGAME_API IGrabInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = Grab)
	void GrabObject(bool bIsGrabbed);

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = Grab)
	bool IsObjectGrabbed() const;
};