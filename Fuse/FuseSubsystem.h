// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FusePDA.h"
#include "FuseSubsystem.generated.h"



UCLASS(Blueprintable)
class THEGAME_API UFuseSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, meta = (ReturnDisplayName = "FuseSuccessful"))
	bool TryFuse(UPrimitiveComponent* Object1, UPrimitiveComponent* Object2, int32 Amount);

private:

	void SetAllRecipesData();
	FName ValidateFuse(UPrimitiveComponent* Object1, UPrimitiveComponent* Object2);
	bool ValidateFuseImpact(UPrimitiveComponent* Object1, UPrimitiveComponent* Object2);
	void Fuse(UPrimitiveComponent* Object1, UPrimitiveComponent* Object2, FName FusedItem, int32 Amount);

	UPROPERTY()
	UFusePDA* FuseDA;

};
