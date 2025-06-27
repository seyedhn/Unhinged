// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Manager/ManagerBase.h"
#include "CollisionSoundsPDA.h"
#include "DamageSoundsPDA.h"
#include "../Messaging/ChannelTags.h"
#include "../Messaging/PayloadStructs.h"
#include "Sound/SoundConcurrency.h"
#include "Sound/SoundAttenuation.h"
#include "FrictionSoundComp.h"
#include "AudioManager.generated.h"




UCLASS(Blueprintable, BlueprintType)
class THEGAME_API AAudioManager : public AManagerBase
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Collision Sounds")
	void PlayCollisionSound(FGameplayTag Channel, const FPayloadCollision& Payload);
	virtual void PlayCollisionSound_Implementation(FGameplayTag Channel, const FPayloadCollision& Payload);

	UFUNCTION(BlueprintCallable, Category = "Collision Sounds")
	void PlayFrictionSound(UPrimitiveComponent* HitComp, UPhysicalMaterial* PhysMat, float Speed);
	void TerminateFrictionSound(const FFrictionSound& FrictionSound);	

	UFUNCTION(BlueprintCallable, Category = "Collision Sounds", meta = (AdvancedDisplay = "2"))
	UFrictionSoundComp* SpawnCollisionSound(USoundBase* Sound, USceneComponent* AttachToComponent, FName AttachPointName = NAME_None,
		FVector Location = FVector(ForceInit), FRotator Rotation = FRotator::ZeroRotator, EAttachLocation::Type LocationType = EAttachLocation::KeepRelativeOffset,
		bool bStopWhenAttachedToDestroyed = true, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f, float StartTime = 0.f,
		USoundAttenuation* AttenuationSettings = nullptr, USoundConcurrency* ConcurrencySettings = nullptr, bool bAutoDestroy = true);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Collision Sounds")
	UPhysicalMaterial* GetPhysMatOfPrimitive(UPrimitiveComponent* HitComp) const;


protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Collision Sounds")
	UCollisionSoundsPDA* CollisionSounds;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Collision Sounds")
	USoundConcurrency* CollisionConcurrency;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Collision Sounds")
	USoundAttenuation* CollisionAttenuation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Collision Sounds")
	USoundConcurrency* FrictionConcurrency;


private:

	UPROPERTY()
	TMap<FFrictionSound, UFrictionSoundComp*> ActiveFrictionSounds;

};
