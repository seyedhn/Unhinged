// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/AssetManager.h"
#include "GlobalConstantsPDA.h"
#include "GlobalConstantsSubsystem.generated.h"


UCLASS(Blueprintable)
class THEGAME_API UGlobalConstants : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	static UGlobalConstants& Get(const UObject* WorldContextObject);

	UFUNCTION()
	void SetGlobalConstantsDataAsset();

	UFUNCTION(BlueprintCallable, Category = "Combustion")
	float MoistureHeatCapacity() const;

	UFUNCTION(BlueprintCallable, Category = "Heat")
	float HeatTickDt() const;

	UFUNCTION(BlueprintCallable, Category = "Heat")
	float AmbienceTemperature() const;

	UFUNCTION(BlueprintCallable, Category = "Aerodynamics")
	float FlightLift() const;
	UFUNCTION(BlueprintCallable, Category = "Aerodynamics")
	void SetFlightLift(const float NewLift);

private:
	UPROPERTY()
	UGlobalConstantsPDA* DA;

};
