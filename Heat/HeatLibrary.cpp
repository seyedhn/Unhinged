// Copyright 2024 Seyed Nasrollahi. All Rights Reserved.

#include "HeatLibrary.h"
#include "../Manager/ManagerSubsystem.h"
#include "../GlobalConstants/GlobalConstantsSubsystem.h"

float UHeatLibrary::ThermalDt(const UObject* WorldContextObject)
{
  return UGlobalConstants::Get(WorldContextObject).HeatTickDt();
}

float UHeatLibrary::MoistureCv(const UObject* WorldContextObject)
{
  return UGlobalConstants::Get(WorldContextObject).MoistureHeatCapacity();
}

float UHeatLibrary::AmbienceTemperature(const UObject* WorldContextObject)
{
  return UGlobalConstants::Get(WorldContextObject).AmbienceTemperature();
}

float UHeatLibrary::KelvinToCelcius(const UObject* WorldContextObject, const float Kelvin)
{
  return Kelvin - 273.f;
}

float UHeatLibrary::CelciusToKelvin(const UObject* WorldContextObject, const float Celcius)
{
  return Celcius + 273;
}
