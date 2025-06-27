// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "../Damage/DamageManager.h"
#include "DamageSoundsPDA.h"
#include "VoiceComp.generated.h"



UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API UVoiceComp : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Damage Sounds")
	void PlayDamageSound(EDamageType DamageType);

	UFUNCTION(BlueprintCallable)
	void OnVitalsUpdated(float HydrationValue, bool Dehydrated, float RadiationValue, bool Radiated, float SanityValue, bool Insane);

	UFUNCTION(BlueprintCallable)
	void OnStaminaUpdated(float Stamina);

	UFUNCTION()
	void OnBreathingEnded();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage Sounds")
	UDamageSoundsPDA* DamageSounds;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage Sounds")
	USoundBase* DamageVO;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage Sounds")
	USoundConcurrency* DamageConcurrency;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage Sounds")
	USoundConcurrency* DamageVOConcurrency;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Breathing Sound")
	USoundBase* BreathingSound;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Radiation Sound")
	USoundBase* RadiationSound;


private:
	UPROPERTY()
	UAudioComponent* Radiation;
	UPROPERTY()
	UAudioComponent* Breathing;

};
