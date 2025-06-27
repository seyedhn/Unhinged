// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "CollisionSoundsPDA.generated.h"



USTRUCT(Blueprintable)
struct FCollisionPair
{
	GENERATED_BODY();

	FCollisionPair() {}

	FCollisionPair(EPhysicalSurface _PhysSurface1, EPhysicalSurface _PhysSurface2)
		: PhysSurface1(_PhysSurface1), PhysSurface2(_PhysSurface2)
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TEnumAsByte<EPhysicalSurface> PhysSurface1 = EPhysicalSurface::SurfaceType2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TEnumAsByte<EPhysicalSurface> PhysSurface2 = EPhysicalSurface::SurfaceType2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TSoftObjectPtr<USoundBase> CollisionSound;

	bool operator==(const FCollisionPair& Other) const
	{
		return (PhysSurface1 == Other.PhysSurface1 && PhysSurface2 == Other.PhysSurface2) || (PhysSurface1 == Other.PhysSurface2 && PhysSurface2 == Other.PhysSurface1);
	}

	friend uint32 GetTypeHash(const FCollisionPair& pair)
	{
		return GetTypeHash(pair.PhysSurface1) + GetTypeHash(pair.PhysSurface2);
	}
};


template<>
struct TStructOpsTypeTraits<FCollisionPair> : public TStructOpsTypeTraitsBase2<FCollisionPair>
{
	enum
	{
		WithIdenticalViaEquality = true,
	};
};



USTRUCT(Blueprintable)
struct FCollisionSingle
{
	GENERATED_BODY();

	FCollisionSingle() {}

	FCollisionSingle(EPhysicalSurface _PhysSurface)
		: PhysSurface(_PhysSurface)
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TEnumAsByte<EPhysicalSurface> PhysSurface = EPhysicalSurface::SurfaceType2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TSoftObjectPtr<USoundBase> CollisionSound;

	bool operator==(const FCollisionSingle& Other) const
	{
		return PhysSurface == Other.PhysSurface;
	}

	friend uint32 GetTypeHash(const FCollisionSingle& pair)
	{
		return uint32(CityHash64((char*)&pair, sizeof(uint8)));
	}
};


UCLASS(Blueprintable)
class THEGAME_API UCollisionSoundsPDA : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	TSoftObjectPtr<USoundBase> GetCollisionSound(UPhysicalMaterial* PhysMat1, UPhysicalMaterial* PhysMat2) const;

	UFUNCTION(BlueprintCallable)
	TSoftObjectPtr<USoundBase> GetFrictionSound(UPhysicalMaterial* PhysMat1) const;

private:

	UPROPERTY(EditAnywhere, Category = "Collision Sounds")
	TSet<FCollisionPair> CollisionSounds;

	UPROPERTY(EditAnywhere, Category = "Collision Sounds")
	TSet<FCollisionSingle> SingleMatCollisionSounds;

	UPROPERTY(EditAnywhere, Category = "Collision Sounds")
	TSet<FCollisionSingle> FrictionSounds;

};
