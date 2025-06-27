// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Manager/ManagerBase.h"
#include "HeatManager.generated.h"




UCLASS(Blueprintable, BlueprintType)
class THEGAME_API AHeatManager : public AManagerBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ReceiveHeat(const UObject* Object, const float Heat);

};
