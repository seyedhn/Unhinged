// Fill out your copyright notice in the Description page of Project Settings.

#include "GlobalConstantsSubsystem.h"

void UGlobalConstants::Initialize(FSubsystemCollectionBase& Collection)
{
  SetGlobalConstantsDataAsset();
}

void UGlobalConstants::Deinitialize()
{
}

UGlobalConstants& UGlobalConstants::Get(const UObject* WorldContextObject)
{
  UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
  check(World);
  UGlobalConstants* GlobalConstants = UGameInstance::GetSubsystem<UGlobalConstants>(World->GetGameInstance());
  check(GlobalConstants);
  return *GlobalConstants;
}

void UGlobalConstants::SetGlobalConstantsDataAsset()
{
  FAssetRegistryModule& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

  FARFilter Filter;
  Filter.bRecursivePaths = true;
  Filter.ClassPaths.Add(UGlobalConstantsPDA::StaticClass()->GetClassPathName());
  //Filter.PackagePaths.Add("/Game/OurContent/Blueprints/Systems/GlobalConstants");

  TArray<FAssetData> AssetData;
  AssetRegistry.Get().GetAssets(Filter, AssetData);

 // UE_LOG(LogTemp, Warning, TEXT("Global Constants Subsystem - Number of UGlobalConstantsPDA data assets found: %i"), AssetData.Num());

  for (FAssetData asset : AssetData)
  {
    UGlobalConstantsPDA* PotentialDA = Cast<UGlobalConstantsPDA>(asset.GetAsset());

    if (PotentialDA)
    {
      DA = PotentialDA;
     // UE_LOG(LogTemp, Warning, TEXT("Global Constants Subsystem - Data Asset successfully set: %i"), AssetData.Num());
      //UAssetManager::Get().UnloadPrimaryAsset(PotentialDA->GetPrimaryAssetId());
      break;
    }
    UE_LOG(LogTemp, Warning, TEXT("Global Constants Subsystem - UGlobalConstantsPDA data asset not valid, or more than one exists: %i"), AssetData.Num());
  }

}

float UGlobalConstants::MoistureHeatCapacity() const
{
  return DA->MoistureHeatCapacity;
}

float UGlobalConstants::HeatTickDt() const
{
  if (DA)
  {
    return DA->ThermalTickDt;
  }
  UE_LOG(LogTemp, Warning, TEXT("Global Constants Subsystem - DA not valid. Returning a value of 0 for HeatTickDt."));
  return 0.f;
}

float UGlobalConstants::AmbienceTemperature() const
{
  return DA->AmbienceTemperature;
}

float UGlobalConstants::FlightLift() const
{
  return DA->FlightLift;
}

void UGlobalConstants::SetFlightLift(const float NewLift)
{
  DA->FlightLift = NewLift;
}
