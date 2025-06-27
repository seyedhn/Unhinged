// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserSettingsActor.generated.h"



UCLASS(Blueprintable)
class THEGAME_API AUserSettingsActor : public AActor
{
	GENERATED_BODY()

public:


	UFUNCTION(BlueprintCallable)
	void SetSoundVolume(class USoundMix* InSoundMixModifier, class USoundClass* InSoundClass, float Volume);


	UFUNCTION(BlueprintImplementableEvent, Category = "Audio Settings")
	void SetMasterVolume(const float Value);
	UFUNCTION(BlueprintImplementableEvent, Category = "Audio Settings")
	void SetAmbienceVolume(const float Value);
	UFUNCTION(BlueprintImplementableEvent, Category = "Audio Settings")
	void SetVoiceVolume(const float Value);


	UFUNCTION(BlueprintImplementableEvent, Category = "Sky Settings")
	void SetTimeOfDay(const float Value);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sky Settings")
	void SetTimeSpeed(const float Value);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sky Settings")
	void SetSunYaw(const float Value);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sky Settings")
	void SetFogDensity(const float Value);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sky Settings")
	void SetCloudCoverage(const float Value);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sky Settings")
	void SetCloudDirection(const float Value);

	UFUNCTION(BlueprintImplementableEvent, Category = "Sky Settings")
	float GetTimeOfDay();
	UFUNCTION(BlueprintImplementableEvent, Category = "Sky Settings")
	float GetTimeSpeed();
	UFUNCTION(BlueprintImplementableEvent, Category = "Sky Settings")
	float GetSunYaw();
	UFUNCTION(BlueprintImplementableEvent, Category = "Sky Settings")
	float GetFogDensity();
	UFUNCTION(BlueprintImplementableEvent, Category = "Sky Settings")
	float GetCloudCoverage();
	UFUNCTION(BlueprintImplementableEvent, Category = "Sky Settings")
	float GetCloudDirection();

	UFUNCTION(BlueprintImplementableEvent, Category = "Graphics Settings")
	void SetNumShadowCascades(const int32 Value);
	UFUNCTION(BlueprintImplementableEvent, Category = "Graphics Settings")
	void SetDynamicShadowDistance(const float Value);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Video Settings")
	void SetPauseBG(const bool Value);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Video Settings")
	void SetPauseGame(const bool Value);
};
