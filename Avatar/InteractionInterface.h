// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionComp.h"
#include "InteractionInterface.generated.h"



UINTERFACE(BlueprintType)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class THEGAME_API IInteractionInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = "Interaction")
	void OnInteract(const EInteractionStatus InteractStatus, const int32 InteractIndex);

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = "Interaction")
	void OnDetected(int32 CompIndex);

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = "Interaction")
	void PrimaryProperties(EInteractionType& Type, FText& Description, float& ProgressLength) const;

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = "Interaction")
	void SecondaryProperties(EInteractionType& Type, FText& Description, float& ProgressLength) const;


};