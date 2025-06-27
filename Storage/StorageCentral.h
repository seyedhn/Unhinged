// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StorageBase.h"
#include "StorageCentral.generated.h"



UCLASS(Blueprintable, BlueprintType)
class THEGAME_API AStorageCentral: public AStorageBase
{
	GENERATED_BODY()


public:
	virtual void BeginPlay() override;
	void AddPoleToCentral(AStorageBase* Pole);
	void EstablishNetwork(FGameplayTag channel, const FEmptyPayload& payload);
	void ResetNetwork();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsWithinStorageRange(AActor* TargetActor, float RangeSquared) const;

protected:


private:
	TArray<AStorageBase*> AllPoles;

};
