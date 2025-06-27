// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Block/BlockActor.h"
#include "PipeNetwork.generated.h"


UENUM(BlueprintType)
enum class EPipeType : uint8
{
	Source	 UMETA(DisplayName = "Source"),
	Pipe  UMETA(DisplayName = "Pipe"),
	Consumer  UMETA(DisplayName = "Consumer"),
};



USTRUCT(Blueprintable)
struct FPipeNetwork
{
	GENERATED_BODY();

	FPipeNetwork() {}

	FPipeNetwork(ABlockActor* source)
	{
		SourceBlocks.Add(source);
	}


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TSet<ABlockActor*> SourceBlocks;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TSet<ABlockActor*> ConsumerBlocks;

	UPROPERTY(SaveGame)
	TArray<FGuid> BlockIDs;

	void AddBlock(ABlockActor* block);

	void SetBlockIDs();

	void SetBlocksFromIDs(UObject* WorldContextObject);

	bool NetworkContainsSource(ABlockActor* source) const;

	bool IsNetworkValid() const;

	TArray<ABlockActor*> GetSourceBlocks() const;

	bool operator==(const FPipeNetwork& Other) const
	{
		return SourceBlocks.Intersect(Other.SourceBlocks).Num() > 0;
	}

	friend uint32 GetTypeHash(const FPipeNetwork& pair)
	{
		return uint32(CityHash64((char*)&pair, sizeof(FPipeNetwork)));
	}

};