// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PostprocessPDA.generated.h"


UENUM(BlueprintType)
enum class EDynamicPPType : uint8
{
	ManualRemove	 UMETA(DisplayName = "ManualRemove"),
	FireAndForget  UMETA(DisplayName = "FireAndForget")
};

USTRUCT(Blueprintable)
struct FPostprocessStruct
{
	GENERATED_BODY();

	FPostprocessStruct() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TSoftObjectPtr<UMaterialInterface> Material;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FName ParameterName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float ParameterValue = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float ActivationLength = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float DeactivationLength = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	EDynamicPPType Type = EDynamicPPType::ManualRemove;

};



UCLASS(Blueprintable)
class THEGAME_API UPostprocessPDA : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	friend class APostprocessManager;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, FPostprocessStruct> PPMaterials;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, FPostProcessSettings> PPSettings;

private:



};
