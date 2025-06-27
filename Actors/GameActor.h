// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Save/SaveGameInterface.h"
#include "GameActor.generated.h"



UCLASS(Blueprintable, BlueprintType)
class THEGAME_API AGameActor : public AActor, public ISaveGameInterface
{
	GENERATED_BODY()

public:
	AGameActor();
	virtual void OnConstruction(const FTransform& Transform) override;
	FGuid GetActorID_Implementation() const override;
	bool SaveAllComps_Implementation() const override;
	void SetActorID_Implementation(FGuid SavedActorID) override;
	virtual void OnActorSpawnLoaded_Internal(bool RewindLoad) override;


protected:

	UPROPERTY(SaveGame)
	FGuid ActorID;

	UPROPERTY(BLueprintReadWrite)
	bool bIsSpawnLoaded;
};
