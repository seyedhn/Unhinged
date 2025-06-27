// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkyConfigsPDA.h"
#include "SkyPDA.generated.h"

USTRUCT(Blueprintable)
struct FSkyStruct
{
	GENERATED_BODY();

	FSkyStruct() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TSoftObjectPtr<USkyConfigsPDA> SkyConfig;

	//The duration that the sky stays in full config, AFTER the transition is completed. A value of 0 results in the sky staying permanently.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables, meta = (DisplayName = "Temporary Sky Duration", EditCondition = "Temporary==True"))
	float Duration = 0.f;

	//The time it takes to interpolate the sky to the new config. A value of 0 results in instant transition.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float ActivationLength = 0.f;

	//The time it takes to interpolate the sky back. A value of 0 results in instant transition.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables, meta = (EditCondition = "Duration>0", EditConditionHides))
	float DeactivationLength = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables, meta = (InlineEditConditionToggle))
	bool ChangeTime = false;

	//If checked and value is set to 0, the default sky time will be applied 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables, meta = (EditCondition = "ChangeTime==True"))
	float Time = 0.f;
};



UCLASS(Blueprintable)
class THEGAME_API USkyPDA : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	friend class APostprocessManager;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, FSkyStruct> SkyConfigs;

private:



};
