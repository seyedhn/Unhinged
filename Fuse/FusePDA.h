// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"
#include "FusePDA.generated.h"


USTRUCT(Blueprintable)
struct FFusePair
{
	GENERATED_BODY();

	FFusePair()
		: Item1(NAME_None), Item2(NAME_None)
	{}

	FFusePair(FName _item1, FName _item2)
		: Item1(_item1), Item2(_item2)
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FName Item1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FName Item2;

	bool operator==(const FFusePair& Other) const
	{
		return (Item1 == Other.Item1 && Item2 == Other.Item2) || (Item1 == Other.Item2 && Item2 == Other.Item1);
	}

	friend uint32 GetTypeHash(const FFusePair& pair)
	{
		return uint32(CityHash64((char*)&pair, sizeof(FFusePair)));
	}

};




USTRUCT(Blueprintable)
struct FFuseRecipe
{
	GENERATED_BODY();

	FFuseRecipe() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float Temp1 = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float Temp2 = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float Impact = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FName FusedItem = NAME_None;

};





UCLASS(Blueprintable)
class THEGAME_API UFusePDA : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	friend class UFuseSubsystem;

	UFUNCTION(BlueprintCallable)
	TMap<FFusePair, FFuseRecipe> GetRecipes() const;

	UFUNCTION(BlueprintCallable)
	FFuseRecipe FindRecipe(FName Item1, FName Item2) const;

private:

	UPROPERTY(EditAnywhere, Category = "Recipes")
	TMap<FFusePair, FFuseRecipe> recipes;

};
