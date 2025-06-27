#include "VitalsComp.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "ItemDataSubsystem.h"
#include "../ItemsData/ItemPropertyStructs.h"
#include "Kismet/GameplayStatics.h"
#include "../Damage/DamageLibrary.h"

UVitalsComp::UVitalsComp()
{
  Hydration = 78.f;
  Sanity = 100.f;
  Radiation = 0.f;
  Health = 100;
  ExposedRadiationRate = 1.f;
}

void UVitalsComp::BeginPlay()
{
  Super::BeginPlay();

  UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);

  MessageSubsystem.RegisterListener(FChannelTags::Get().ItemConsumed, this, &UVitalsComp::OnItemConsumed);

  GetWorld()->GetTimerManager().SetTimer(VitalsTimer, this, &UVitalsComp::RunVitals, Dt, true);
}

void UVitalsComp::OnActorReloaded_Implementation(bool RewindLoad)
{
  UpdateRadiation(-100);
}

void UVitalsComp::RunVitals()
{
  RegenHealth();
  RegenStamina();
  DrainHydration();
  ExposeToRadiation();
  DrainSanity();
  OnVitalsUpdated.Broadcast(Hydration, bDehydrated, Radiation, bRadiated, Sanity, bInsane);
}


//---------------------HYDRATION------------------------------//

void UVitalsComp::OnItemConsumed(FGameplayTag Channel, const FPayloadKey& Payload)
{
  UItemDataSubsystem& ItemDataSubsystem = UItemDataSubsystem::Get(this);

  FItemFoodData FoodData = ItemDataSubsystem.GetItemProperty<FItemFoodData>(Payload.Key);
  USoundBase* Sound = FoodData.HealthValue > FoodData.CaloricValue ? EatingSound : DrinkingSound;
  UGameplayStatics::PlaySound2D(this, Sound, 1.f, 1.f, 0.f, nullptr, nullptr, false);

  UpdateHealth(FoodData.HealthValue);
  UpdateHydration(FoodData.CaloricValue);
  UpdateRadiation(FoodData.RadiationValue);
  UpdateSanity(FoodData.SanityValue);
}

void UVitalsComp::SetVitals(const float HydrationLevel, const float RadiationLevel)
{
  Hydration = HydrationLevel;
  Radiation = RadiationLevel;
}

void UVitalsComp::UpdateHydration(const float HydrationValue)
{
  Hydration = FMath::Clamp(Hydration + HydrationValue, 0.f, HydrationMax);
}

void UVitalsComp::DrainHydration()
{

  UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
  if (XOR(bDehydrated, Hydration < HydrationCriticality))
  {
    bDehydrated = !bDehydrated;
    MessageSubsystem.BroadcastMessage(FChannelTags::Get().PPActivateMaterial, FPayloadKeyBool(TEXT("Hunger"), bDehydrated));
    if(bDehydrated) MessageSubsystem.BroadcastMessage(FChannelTags::Get().Warning, FPayloadKey(TEXT("Hunger")));
  }

  if (Hydration > 0)
  {
    UpdateHydration(-DehydrationRate * Dt);
  }
  else
  {
    DamageReceived(DehydrationDamage * Dt, EDamageType::Pure);
  }
}


//---------------------RADIATION------------------------------//

void UVitalsComp::ReceiveRadiationDamage(float _RadiationDamage)
{
  UpdateRadiation(_RadiationDamage);
}

void UVitalsComp::UpdateRadiation(const float RadiationValue)
{
  Radiation = FMath::Clamp(Radiation + RadiationValue, 0, 100);

  if (XOR(bRadiated, Radiation > RadiationCriticality))
  {
    bRadiated = !bRadiated;
    OnRadiated.Broadcast(bRadiated);
    if(bRadiated) UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().Warning, FPayloadKey(TEXT("Radiation")));
  }
  if (Radiation > RadiationCriticality)
  {
    DamageReceived(RadiationDamage * Dt * (Radiation-RadiationCriticality)/100, EDamageType::Pure);
  }
}

void UVitalsComp::ExposeToRadiation()
{
  float RadiationChange = ExposedRadiationRate - RadiationDrainRate;
  UpdateRadiation(RadiationChange * Dt);
}

void UVitalsComp::SetExposedToRadiation(const bool InShelter, const float ExposureRate)
{
  if (InShelter) ExposedRadiationRate = ExposureRate;
  else ExposedRadiationRate = BGRadiationRate;
  OnRadiationExposureUpdated.Broadcast(ExposedRadiationRate, !InShelter);
}



//---------------------HEALTH------------------------------//

void UVitalsComp::UpdateHealth(const float HealthValue)
{
  Health = FMath::Clamp(Health + HealthValue, 0.f, MaxHealth);
}

void UVitalsComp::RegenHealth()
{
  if (Health < MaxHealth * HealthRegenThreshold)
  {
    UpdateHealth(HealthRegenRate * Dt);
    OnHealthGained.Broadcast(HealthPercentage(), HealthCritical());
  }
}

void UVitalsComp::BecomeInvincible(bool Invincible)
{
  bInvincible = Invincible;
}

float UVitalsComp::HealthPercentage() const
{
  return Health / MaxHealth;
}

bool UVitalsComp::HealthCritical() const
{
  return  Health < MaxHealth * HealthCriticality;
}

void UVitalsComp::ReceiveDamage_Implementation(UPrimitiveComponent* HitComp, float HitDamage, EDamageType DamageType, int32 InstanceIndex, FVector HitLocation, FVector HitNormal, FName HitBone)
{
  if (HitDamage > 0 && DamageMultipliers.Contains(DamageType) && Health > 0 && !bInvincible)
  {
    float Damage = DamageMultipliers.FindRef(DamageType) * HitDamage;
    DamageReceived(Damage, DamageType);

    if (DamageType == EDamageType::Radiation) ReceiveRadiationDamage(Damage);
  }
}

void UVitalsComp::DamageReceived(float Damage, EDamageType DamageType)
{
  UpdateHealth(-Damage);
  OnDamageReceived.Broadcast(Damage, HealthPercentage(), DamageType);
  if (Health <= 0)
  {
    OnDeath.Broadcast();
  }
}



//---------------------STAMINA------------------------------//

void UVitalsComp::DrainStamina(float DrainPoints)
{
  Stamina -= DrainPoints;
  OnStaminaUpdated.Broadcast(StaminaPercentage());
  if (Stamina <= 0)
  {
    OnOutOfStamina.Broadcast();
  }
}

void UVitalsComp::RegenStamina()
{
  if (Stamina < StaminaMax)
  {
    Stamina += StaminaRegenRate * Dt;
    OnStaminaUpdated.Broadcast(StaminaPercentage());
  }
}

float UVitalsComp::StaminaPercentage() const
{
  return Stamina / StaminaMax;
}




//---------------SANITY---------------------------------//

void UVitalsComp::UpdateSanity(const float SanityValue)
{
  Sanity = FMath::Clamp(Sanity + SanityValue, 0, 100);

  if(bInsane != Sanity < SanityCriticality) //equivalent of XOR gate
  {
    bInsane = !bInsane;
    OnInsane.Broadcast(bInsane);
    if (bInsane) UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().Warning, FPayloadKey(TEXT("Sanity")));
  }
}

void UVitalsComp::DrainSanity()
{
  UpdateSanity(-SanityDrainRate * Dt);
}

bool UVitalsComp::CanSleep() const
{
    return !bDehydrated;
}

void UVitalsComp::OnWake(int32 Hours)
{
  if (Hours == -1)
  {
      RecoverInBed();
      return;
  }

  Stamina = StaminaMax;
  UpdateHydration(-Hours * 7.5);
  UpdateRadiation((ExposedRadiationRate - RadiationDrainRate) * Hours * 25);
  UpdateSanity(Hours * 5);

}

void UVitalsComp::OnRecall()
{
    Stamina = 0.f;
    UpdateHydration(-2 * 7.5);
    UpdateSanity(-2 * 10);
}

void UVitalsComp::RecoverInBed()
{
    Stamina = StaminaMax;
    UpdateHydration(-8 * 7.5);
    UpdateRadiation((ExposedRadiationRate - RadiationDrainRate) * 8 * 25);
    UpdateSanity(-8 * 10);
}



//OLD RADIATION CODE
/*
void UVitalsComp::UpdateRadiation(const float RadiationValue)
{
  Radiation = FMath::Clamp(Radiation + RadiationValue, 0, 100);

  if (XOR(bRadiated, Radiation > 0))
  {
    bRadiated = !bRadiated;
    if (bRadiated)
    {
      FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([this] {
        //  UpdateHydration(-1.f);
        UpdateRadiation(-1.f);
        });

      OnRadiated.Broadcast(true);
      GetWorld()->GetTimerManager().SetTimer(RadiationTimer, TimerDelegate, 1.f, true);
    }
    else
    {
      OnRadiated.Broadcast(false);
      GetWorld()->GetTimerManager().ClearTimer(RadiationTimer);
    }
  }
  OnVitalsUpdated.Broadcast(Hydration, bDehydrated, Radiation, bRadiated);
}*/