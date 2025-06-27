// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VoiceOverPDA.generated.h"



UCLASS(Blueprintable)
class THEGAME_API UVoiceOverPDA : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Voice Over")
	TMap<FName, TSoftObjectPtr<USoundBase>> VO;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Voice Over")
	TMap<FName, FName> VoiceOverToHint;

private:





};
