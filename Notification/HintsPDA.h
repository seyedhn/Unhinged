// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"
#include "HintsPDA.generated.h"



USTRUCT(Blueprintable)
struct FHint
{
	GENERATED_BODY();

	FHint() {}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Hint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon;
};

UCLASS(Blueprintable)
class THEGAME_API UHintsPDA : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, FHint> Hints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, FName> FollowUps;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, FText> TitleCards;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, FText> Warnings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, FText> Objectives;


};
