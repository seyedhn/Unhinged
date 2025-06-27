// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGameInterface.generated.h"



UINTERFACE(MinimalAPI)
class USaveGameInterface : public UInterface
{
	GENERATED_BODY()
};

class THEGAME_API ISaveGameInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = SaveGame)
	void SetActorID(FGuid SavedActorID);

	UFUNCTION(BlueprintNativeEvent, Category = SaveGame)
	FGuid GetActorID() const;

	UFUNCTION(BlueprintNativeEvent, Category = SaveGame)
	bool SaveAllComps() const;

	UFUNCTION(BlueprintNativeEvent, Category = SaveGame)
	void OnActorSaved();

	UFUNCTION(BlueprintNativeEvent, Category = SaveGame)
	void OnActorReloaded(bool RewindLoad);

	UFUNCTION(BlueprintNativeEvent, Category = SaveGame)
	void OnActorSpawnLoaded(bool RewindLoad);

	virtual void OnActorSpawnLoaded_Internal(bool RewindLoad) {};

};