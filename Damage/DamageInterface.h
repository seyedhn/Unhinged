// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageInterface.generated.h"



UINTERFACE(MinimalAPI)
class UDamageInterface : public UInterface
{
	GENERATED_BODY()
};

class THEGAME_API IDamageInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = Damage)
	void ReceiveDamage(UPrimitiveComponent* HitComp, float HitDamage, EDamageType DamageType, int32 InstanceIndex, FVector HitLocation, FVector HitNormal, FName HitBone);

};