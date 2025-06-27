// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"
#include "TrialsPDA.generated.h"


USTRUCT(Blueprintable)
struct FTrialRewards
{
	GENERATED_BODY();

	FTrialRewards() {}
	FTrialRewards(FName reward)
	{
		Rewards.Add(reward);
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TMap<FName, int32> Rewards;
};


UCLASS(Blueprintable)
class THEGAME_API UTrialsPDA : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<int32> TierPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FTrialRewards> TrialRewards;
};
