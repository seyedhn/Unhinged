// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "NiagaraSystem.h"
#include "FoliagePDA.generated.h"



UCLASS(Blueprintable)
class THEGAME_API UFoliagePDA : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FoliageFX", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	TSoftObjectPtr<UNiagaraSystem> GetVFX(const UObject* WorldContextObject, const FName Key) const;

private:

	UPROPERTY(EditAnywhere, Category = "FoliageFX")
	TMap<FGameplayTag, TSoftObjectPtr<UNiagaraSystem>> VFX;



};
