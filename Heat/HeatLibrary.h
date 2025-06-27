// Copyright 2024 Seyed Nasrollahi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include  "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "HeatLibrary.generated.h"


UCLASS(Blueprintable)
class THEGAME_API UHeatLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Thermal", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", ReturnDisplayName = "Dt"))
	static float ThermalDt(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Thermal", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", ReturnDisplayName = "MoistureCv"))
	static float MoistureCv(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Thermal", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", ReturnDisplayName = "Kelvin"))
	static float AmbienceTemperature(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Thermal", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", ReturnDisplayName = "Celcius"))
	static float KelvinToCelcius(const UObject* WorldContextObject, const float Kelvin);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Thermal", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", ReturnDisplayName = "Kelvin"))
	static float CelciusToKelvin(const UObject* WorldContextObject, const float Celcius);


};