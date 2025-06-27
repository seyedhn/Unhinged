// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RewindInterface.generated.h"

class URewindComp;


UINTERFACE(MinimalAPI)
class URewindInterface : public UInterface
{
	GENERATED_BODY()
};

class THEGAME_API IRewindInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = Rewind)
	bool IsCharged(URewindComp*& RewindComp) const;

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = Rewind)
	bool IsRewindable() const;

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = Rewind)
	void BecomeCharged(bool Charged, UPrimitiveComponent* HitComp);


};