// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FoliageInstancedStaticMeshComponent.h"
#include "../Save/SaveGameInterface.h"
#include "../Heat/CombustionManager.h"
#include "FoliageISMC.generated.h"

UENUM(BlueprintType)
enum class EFoliageType : uint8
{
	Plant	 UMETA(DisplayName = "Plant"),
	SmallTree  UMETA(DisplayName = "SmallTree"),
	MediumTree  UMETA(DisplayName = "MediumTree"),
	LargeTree UMETA(DisplayName = "LargeTree"),
	Rock UMETA(DisplayName = "Rock")
};

USTRUCT(Blueprintable) 
struct FFoliageTransformStruct
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	int32 Index = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FTransform Transform;
};






UCLASS(Blueprintable, BlueprintType)
class THEGAME_API UFoliageISMC : public UFoliageInstancedStaticMeshComponent, public ISaveGameInterface
{
	GENERATED_BODY()

public:

	void OnActorReloaded_Implementation(bool RewindLoad)  override;
	void OnActorSaved_Implementation() override;

	TArray<int32> GetRemovedInstances() const;
	void SetRemovedInstance(TArray<int32> instances);

	UFUNCTION(BlueprintCallable)
	bool IsBurntOrBurning(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	void ExtinguishFire(const int32 Index);


protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<int32> RemovedInstances;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FFoliageTransformStruct> TempRemovedInstances;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSet<int32> Burnt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<int32, FBurnFX> Burning;

};
