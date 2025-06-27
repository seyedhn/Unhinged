#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "../Upgrade/UpgradeManager.h"
#include "../Damage/DamageManager.h"
#include "../Spline/PipeNetwork.h"
#include "../Foliage/FoliageISMC.h"
#include "../Avatar/InteractionComp.h"
#include "ItemDataAsset.h"
#include "ItemPropertyStructs.generated.h"



USTRUCT(Blueprintable)
struct FItemInventoryData : public FItemInstancedProperty
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables, meta=(MultiLine = true))
	FText Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TSoftObjectPtr<UTexture2D> LargeIcon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	int32 StackAmount = 1;

};

USTRUCT(Blueprintable) //Metals, Alloys, Blocks
struct FItemPhysicalProperties : public FItemInstancedProperty
{

	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float Mass = 1.f;

};


USTRUCT(Blueprintable)
struct FItemDropData : public FItemInstancedProperty
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TMap<FName, int32> Drops;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TMap<FName, float> RandomDrops;

};


USTRUCT(Blueprintable)
struct FItemPurchaseData : public FItemInstancedProperty
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	int32 Tier = 1;

	//Sellable items need to have negative costs
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	int32 Cost = 1;

	//Upgrades need to have negative Amounts
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables, meta = (EditCondition = "Cost>=0", EditConditionHides))
	int32 DeliveredAmount = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables, meta = (EditCondition = "DeliveredAmount<0 && Cost>=0", EditConditionHides))
	TArray<FName> DeliveredItems;

	EPurchaseType GetPurchaseType() const;
};


USTRUCT(Blueprintable)
struct FItemCombustion : public FItemInstancedProperty
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float CombustibleMass = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float EnergyDensity = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float BurnRate = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float MoistureContent = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FName ProcessItem;

};


USTRUCT(Blueprintable)
struct FItemHealth : public FItemInstancedProperty //Blocks, Equipment
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float Health = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TMap<EDamageType, float> DamageMultiplier;
};


USTRUCT(Blueprintable)
struct FItemDamager : public FItemInstancedProperty //Blocks, Equipment
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float Damage = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	EDamageType DamageType = EDamageType::None;
};


USTRUCT(Blueprintable)
struct FItemFoodData : public FItemInstancedProperty
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float HealthValue = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float CaloricValue = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float RadiationValue = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float SanityValue = 0.f;
};


USTRUCT(Blueprintable)
struct FItemExplosive : public FItemInstancedProperty
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float Radius = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float Impact = 0;
};


USTRUCT(Blueprintable)
struct FItemInteractionData : public FItemInstancedProperty
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	EInteractionType InteractionType = EInteractionType::Interactable;
};


USTRUCT(Blueprintable)
struct FItemPipeData : public FItemInstancedProperty
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	EPipeType PipeType = EPipeType::Source;
};


USTRUCT(Blueprintable)
struct FItemFluidData : public FItemInstancedProperty
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FLinearColor Colour = FLinearColor(1.f, 1.f, 1.f);
};








USTRUCT(Blueprintable)
struct FItemChargeData : public FItemInstancedProperty
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	int32 MaxCapacity = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float ChargeRate = 0.f;

};


USTRUCT(Blueprintable)
struct FItemThermalProperties : public FItemInstancedProperty //Metals, Alloys, Blocks, Ores, ScrapMetals
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float Density = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float HeatCapacity = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float ThermalConductivity = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float ProcessTemperature = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FName ProcessItem;
};


USTRUCT(BlueprintType)
struct FTrialData : public FItemInstancedProperty
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 RewardGroup = 0;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 TrialPoints1 = 0;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 TrialPoints2 = 0;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float TimeLimit2 = 0;


};