// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Manager/ManagerBase.h"
#include "../Messaging/PayloadStructs.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "CombustionManager.generated.h"


USTRUCT(Blueprintable)
struct FCombustible
{
	GENERATED_BODY();

	FCombustible() {}

	FCombustible(UObject* _Object)
		: Object(_Object)
	{}

	FCombustible(UObject* _Object, int32 _Index)
		: Object(_Object), Index(_Index)
	{}

	FCombustible(UObject* _Object, int32 _Index, float _Mass, float _Moisture)
		: Object(_Object), Index(_Index), Mass(_Mass), Moisture(_Moisture)
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	UObject* Object = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	int32 Index = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float Mass = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float Moisture = 0.f;

	bool operator==(const FCombustible& Other) const
	{
		return (Object == Other.Object && Index == Other.Index);
	}

	friend uint32 GetTypeHash(const FCombustible& pair)
	{
		return GetTypeHash(pair.Object) + GetTypeHash(pair.Index);
	}
};

USTRUCT(Blueprintable)
struct FBurnFX
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	UAudioComponent* Sound = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	UFXSystemComponent* VFX = nullptr;
};


UCLASS(Blueprintable, BlueprintType)
class THEGAME_API ACombustionManager : public AManagerBase
{
	GENERATED_BODY()

public:
	virtual void PreLoadManager(bool bRewindLoad) override;

	UFUNCTION(BlueprintCallable)
	void ReceiveBurn(UObject* Object, const int32 Index, const float Burn);

	UFUNCTION(BlueprintCallable)
	void AddCombustible(const FCombustible& Combustible);

	void UpdateMass(const FCombustible& Combustible, const float NewMass);

	void DealBurnToSurrounding(const FCombustible& Combustible, const float EnergyDensity, const float BurnRate, const float Dt);
	bool SelfCombustion(const FCombustible& Combustible, const float FullMass, const float BurnRate, const float Dt);

	UFUNCTION()
	void BurnTick();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	TSet<FCombustible> Combustibles;

	UPROPERTY()
	FTimerHandle BurnTimer;
};
