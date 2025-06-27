// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Actors/GameActor.h"
#include "ResourceBox.generated.h"



UCLASS(Blueprintable, BlueprintType)
class THEGAME_API AResourceBox : public AGameActor
{
	GENERATED_BODY()

public:
	void SetItems(const TMap<FName, int32>& ResourceItems);

protected:

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn=true))
	TMap<FName, int32> Items;


	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere)
	TSet<FName> CollectedItems;
};
