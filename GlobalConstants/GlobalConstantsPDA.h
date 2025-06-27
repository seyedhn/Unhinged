// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"
#include "GlobalConstantsPDA.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class THEGAME_API UGlobalConstantsPDA : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	friend class UGlobalConstants;


private:

	UPROPERTY(EditAnywhere, Category = "Thermal", AssetRegistrySearchable)
	float ThermalTickDt;

	UPROPERTY(EditAnywhere, Category = "Thermal")
	float MoistureHeatCapacity;

	UPROPERTY(EditAnywhere, Category = "Thermal", meta = (DisplayName = "Ambience Temperature (K)"))
	float AmbienceTemperature;

	UPROPERTY(EditAnywhere, Category = "Aerodynamics")
	float FlightLift;


};
