// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Manager/ManagerBase.h"
#include "SkyPDA.h"
#include "SkyConfigsPDA.h"
#include "SkyManager.generated.h"




UCLASS(Blueprintable, BlueprintType)
class THEGAME_API ASkyManager : public AManagerBase
{
	GENERATED_BODY()

public:

	void LoadManager(bool bRewindLoad) override;
	void SaveManager() override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Initialise")
	void SetUDS();

	UFUNCTION()
	void SetSkyConfigName();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DeterminesOutputType = "Class", DynamicOutputParam = "OutConfig"))
	bool GetSkyConfigs(TSubclassOf<UPrimaryDataAsset> Class, FName ConfigName, UPrimaryDataAsset*& OutConfig) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Static Sky Change")
	void ApplySkyConfig(const TSoftObjectPtr<USkyConfigsPDA>& SkyConfig);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Static Sky Change")
	void ApplySkyConfigByName(FName PresetName);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Static Sky Change")
	void ChangeDayLength(float DayLength);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Time")
	void ChangeTimeOfDay(float NewTime);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Time")
	void SaveTimeOfDay();

protected:

	UPROPERTY(BlueprintReadWrite)
	TSoftObjectPtr<USkyConfigsPDA> CurrentSky;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	FName CurrentSkyName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USkyPDA* SkyPDA;

	UPROPERTY(BlueprintReadWrite)
	int32 TimeOfDayIndex;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	float CurrentTime;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	TSet<FName> UsedSkies;

};



