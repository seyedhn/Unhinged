// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceSMC.h"
#include "../Actors/GameActor.h"
#include "ResourceContainer.generated.h"



UCLASS(Blueprintable, BlueprintType)
class THEGAME_API AResourceContainer : public AGameActor
{
	GENERATED_BODY()

public:
	AResourceContainer();
	void BeginPlay() override;

	void SpawnItem(const FName Key, const int32 Amount, const FVector Location, const FVector Impulse);

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UResourceSMC> SpawnClass;

};
