// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Save/SaveGameInterface.h"
#include "../Messaging/ChannelTags.h"
#include "../Messaging/PayloadStructs.h"
#include "../Damage/DamageManager.h"
#include "../Damage/DamageInterface.h"
#include "VitalsComp.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FVitalsDelegate, float, HydrationValue, bool, Dehydrated, float, RadiationValue, bool, Radiated, float, SanityValue, bool, Insanity);

//SANITY//
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSanityDelegate, bool, bInsanity);

//RADIATION//
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRadiationDelegate, bool, bRadiated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRadiationExposureDelegate, float, RadiationRate, bool, bCritical);

//HEALTH & DAMAGE//
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDamageDelegate, float, Damage, float, HealthPercentage, EDamageType, DamageType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthDelegate, float, HealthPercentage, bool, Critical);

//STAMINA//
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStaminaUpdateDelegate, float, StaminaPercentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStaminaDrainedDelegate);



UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API UVitalsComp : public UActorComponent, public ISaveGameInterface, public IDamageInterface
{
	GENERATED_BODY()


public:
	UVitalsComp();
	virtual void BeginPlay() override;
	void OnActorReloaded_Implementation(bool RewindLoad) override;
	void RunVitals();


	//HEALTH
	UFUNCTION(BlueprintCallable)
	float HealthPercentage() const;
	UFUNCTION(BlueprintCallable)
	bool HealthCritical() const;
	void ReceiveDamage_Implementation(UPrimitiveComponent* HitComp, float HitDamage, EDamageType DamageType, int32 InstanceIndex, FVector HitLocation, FVector HitNormal, FName HitBone);
	void DamageReceived(float Damage, EDamageType DamageType);
	void UpdateHealth(const float HealthValue);
	void RegenHealth();
	void BecomeInvincible(bool Invincible);

	//STAMINA
	void DrainStamina(float DrainPoints);
	void RegenStamina();
	UFUNCTION(BlueprintCallable)
	float StaminaPercentage() const;

	//HUNGER
	void OnItemConsumed(FGameplayTag Channel, const FPayloadKey& Payload);
	UFUNCTION(BlueprintCallable)
	void SetVitals(const float HydrationLevel, const float RadiationLevel);
	UFUNCTION(BlueprintCallable)
	void UpdateHydration(const float HydrationValue);
	void DrainHydration();

	//RADIATION
	UFUNCTION(BlueprintCallable)
	void ReceiveRadiationDamage(float RadiationDamage);
	void UpdateRadiation(const float RadiationValue);
	void ExposeToRadiation();
	UFUNCTION(BlueprintCallable)
	void SetExposedToRadiation(const bool InShelter, const float ExposureRate);

	//SANITY
	void UpdateSanity(const float SanityValue);
	void DrainSanity();
	UFUNCTION(BlueprintCallable)
	bool CanSleep() const;
	UFUNCTION(BlueprintCallable)
	void OnWake(int32 Hours);
	UFUNCTION(BlueprintCallable)
	void OnRecall();
	void RecoverInBed();


	//DELEGATES
	UPROPERTY(BlueprintAssignable)
	FVitalsDelegate OnVitalsUpdated;
	UPROPERTY(BlueprintAssignable)
	FRadiationDelegate OnRadiated;
	UPROPERTY(BlueprintAssignable)
	FSanityDelegate OnInsane;
	UPROPERTY(BlueprintAssignable)
	FRadiationExposureDelegate OnRadiationExposureUpdated;
	UPROPERTY(BlueprintAssignable)
	FDamageDelegate OnDamageReceived;
	UPROPERTY(BlueprintAssignable)
	FDeathDelegate OnDeath;
	UPROPERTY(BlueprintAssignable)
	FHealthDelegate OnHealthGained;
	UPROPERTY(BlueprintAssignable)
	FStaminaUpdateDelegate OnStaminaUpdated;
	UPROPERTY(BlueprintAssignable)
	FStaminaDrainedDelegate OnOutOfStamina;




	UPROPERTY(EditDefaultsOnly)
	USoundBase* DrinkingSound;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* EatingSound;

	FTimerHandle VitalsTimer;
	FTimerHandle RadiationTimer;

	UPROPERTY(EditDefaultsOnly)
	float Dt;

	//HUNGER
	UPROPERTY(BlueprintReadOnly, SaveGame)
	float Hydration;
	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bDehydrated = false;
	UPROPERTY(EditDefaultsOnly, Category = "Hunger")
	float HydrationMax;
	UPROPERTY(EditDefaultsOnly, Category = "Hunger")
	float HydrationCriticality;
	UPROPERTY(EditDefaultsOnly, Category = "Hunger")
	float DehydrationRate;
	UPROPERTY(EditDefaultsOnly, Category = "Hunger")
	float DehydrationDamage;


	//SANITY
	UPROPERTY(BlueprintReadOnly, SaveGame)
	float Sanity;
	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bInsane = false;
	UPROPERTY(EditDefaultsOnly, Category = "Sanity")
	float SanityCriticality;
	UPROPERTY(EditDefaultsOnly, Category = "Sanity")
	float SanityDrainRate;


	//RADIATION
	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Radiation")
	float Radiation;
	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Radiation")
	bool bRadiated;
	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Radiation")
	float ExposedRadiationRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Radiation")
	float RadiationCriticality;
	UPROPERTY(EditDefaultsOnly, Category = "Radiation")
	float BGRadiationRate;
	UPROPERTY(EditDefaultsOnly, Category = "Radiation")
	float RadiationDrainRate;
	UPROPERTY(EditDefaultsOnly, Category = "Radiation")
	float RadiationDamage;





	//HEALTH
	UPROPERTY(BlueprintReadOnly, SaveGame)
	float Health;
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	TMap<EDamageType, float> DamageMultipliers;
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float HealthRegenThreshold;
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float HealthCriticality;
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float HealthRegenRate;
	UPROPERTY(BlueprintReadOnly)
	bool bInvincible;


	//STAMINA
	float Stamina;
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float StaminaMax;
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float StaminaRegenRate;




};
