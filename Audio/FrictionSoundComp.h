// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "FrictionSoundComp.generated.h"

class AAudioManager;



USTRUCT(Blueprintable)
struct FFrictionSound
{
	GENERATED_BODY()

	FFrictionSound() {}

	FFrictionSound(UPrimitiveComponent* _HitComp, EPhysicalSurface _PhysSurface)
		: HitComp(_HitComp), PhysSurface(_PhysSurface)
	{}

	/*
	FFrictionSound(UPrimitiveComponent* _HitComp, EPhysicalSurface _PhysSurface, UFrictionSoundComp* _FrictionSound)
		: HitComp(_HitComp), PhysSurface(_PhysSurface), FrictionSound(_FrictionSound)
	{}*/

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	UPrimitiveComponent* HitComp = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TEnumAsByte<EPhysicalSurface> PhysSurface = EPhysicalSurface::SurfaceType2;

//	class UFrictionSoundComp* FrictionSound;

	bool operator==(const FFrictionSound& Other) const
	{
		return PhysSurface == Other.PhysSurface && HitComp == Other.HitComp;
	}

	friend uint32 GetTypeHash(const FFrictionSound& pair)
	{
		return GetTypeHash(pair.PhysSurface) + GetTypeHash(pair.HitComp);
	}
};




UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API UFrictionSoundComp : public UAudioComponent
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	void Initialise(AAudioManager* AudioManager, const FFrictionSound& FrictionSound);

	UFUNCTION(BlueprintCallable)
	void CollisionDetected(float Speed);


private:

	void SetTimer();
	void StopSound();

	FTimerHandle StopTimer;
	AAudioManager* AudioManager;
	FFrictionSound FrictionSound;
	

};




