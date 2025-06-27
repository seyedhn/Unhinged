// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"
#include "MediaSource.h"
#include "HallucinationPDA.generated.h"


USTRUCT(Blueprintable)
struct FHallucination
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TSoftObjectPtr<UMediaSource> MediaSource;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	bool MaskedOpacity = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables, meta = (InlineEditConditionToggle))
	bool FadeIn = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables, meta = (EditCondition = "FadeIn==True"))
	float FadeRate = 1.f;

};


UCLASS(Blueprintable)
class THEGAME_API UHallucinationPDA : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FName, FHallucination> Hallucinations;

private:



};
