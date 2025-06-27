// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StructUtils/InstancedStruct.h"
#include "SettingsPDA.generated.h"

UENUM(BlueprintType)
enum class ESettingType : uint8
{
	Boolean	 UMETA(DisplayName = "Boolean"),
	Integer  UMETA(DisplayName = "Integer"),
	Decimal  UMETA(DisplayName = "Decimal")
};

USTRUCT(BlueprintType)
struct FSettingInstancedProperty
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings", meta = (DisplayPriority = "1"))
	FText Title;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings", meta = (DisplayAfter = "Title"))
	FName Name;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings", meta = (DisplayAfter = "Name", MultiLine=true))
	FText Description;
};

USTRUCT(Blueprintable)
struct FSettingsMP : public FSettingInstancedProperty
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings")
	TArray<FText> Values;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings", meta=(ArrayClamp = "Values"))
	int32 DefaultValue = 0;
};

USTRUCT(Blueprintable)
struct FSettingsBinary : public FSettingInstancedProperty
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings")
	bool DefaultValue = false;
};

USTRUCT(Blueprintable)
struct FSettingsScalability : public FSettingInstancedProperty
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings", meta=(ClampMin=0, UIMin=0, ClampMax=3, UIMax=3))
	int32 DefaultValue = 2;
};

USTRUCT(Blueprintable)
struct FSettingsSlider : public FSettingInstancedProperty
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings")
	FVector2D Range = {0.f,1.f};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings", meta=(UIMin=0))
	float DefaultValue = 0;
};



UCLASS(Blueprintable)
class THEGAME_API USettingsPDA : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings", meta = (BaseStruct = "/Script/TheGame.SettingInstancedProperty", ExcludeBaseStruct, DisplayName = "Setting Properties"))
	TArray<FInstancedStruct> SettingProperties;

	UFUNCTION(BlueprintCallable)
	ESettingType GetSettingType(FInstancedStruct Setting) const;


};
