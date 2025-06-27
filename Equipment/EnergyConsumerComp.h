// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnergyConsumerComp.generated.h"


UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API UEnergyConsumerComp : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool TryConsumeCharge(int32 Amount);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Charge;
};
