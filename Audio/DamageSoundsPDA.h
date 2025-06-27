// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../Damage/DamageManager.h"
#include "DamageSoundsPDA.generated.h"



UCLASS(Blueprintable)
class THEGAME_API UDamageSoundsPDA : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Damage Sounds")
	TMap<EDamageType, TSoftObjectPtr<USoundBase>> DamageSounds;

private:





};
