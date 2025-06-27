#include "CombustionManager.h"
#include "ItemDataSubsystem.h"
#include "../Heat/HeatLibrary.h"
#include "../ItemsData/ItemPropertyStructs.h"
#include "CombustionInterface.h"
#include "../Damage/DamageLibrary.h"

void ACombustionManager::PreLoadManager(bool bRewindLoad)
{
  //This extinguishes all active combustibles. The proper way is to differentiate between those that were burning at save as well
  GetWorld()->GetTimerManager().PauseTimer(BurnTimer);
  for (auto& combustible : Combustibles)
  {
    if(IsValid(combustible.Object)) ICombustionInterface::Execute_BurnExtinguished(combustible.Object, combustible.Index);
  }
  Combustibles.Empty();
}

void ACombustionManager::ReceiveBurn(UObject* Object, const int32 Index, const float Burn)
{
  if (!Combustibles.Contains(FCombustible(Object, Index)))
  {
    FItemCombustion CombustionData = UItemDataSubsystem::Get(this).GetItemProperty<FItemCombustion>(Object);
    float MoistureContent = CombustionData.MoistureContent * UHeatLibrary::MoistureCv(this);
    FCombustible Combustible = FCombustible(Object, Index, CombustionData.CombustibleMass, MoistureContent);
    AddCombustible(Combustible);
  }

  FCombustible* Combustible = Combustibles.Find(FCombustible(Object, Index));
  if (Combustible->Moisture > 0)
  {
    Combustible->Moisture = Combustible->Moisture - Burn;
    if (Combustible->Moisture <= 0.f)
    {
      ICombustionInterface::Execute_BurnStarted(Combustible->Object, Combustible->Index);
      GetWorld()->GetTimerManager().SetTimer(BurnTimer, this, &ACombustionManager::BurnTick, UHeatLibrary::ThermalDt(this), true);
    }
  }
}

void ACombustionManager::AddCombustible(const FCombustible& Combustible)
{
  Combustibles.Add(Combustible);
}

void ACombustionManager::UpdateMass(const FCombustible& _Combustible, const float NewMass)
{
  FCombustible* Combustible = Combustibles.Find(_Combustible);
  Combustible->Mass = NewMass;
}

void ACombustionManager::DealBurnToSurrounding(const FCombustible& Combustible, const float EnergyDensity, const float BurnRate, const float Dt)
{
  UPrimitiveComponent* Comp = Cast<UPrimitiveComponent>(Combustible.Object);
  float Damage = EnergyDensity * BurnRate * Dt;
  UDamageLibrary::DealOverlapDamage(this, Comp, Damage, EDamageType::Burn, Combustible.Index);
}

bool ACombustionManager::SelfCombustion(const FCombustible& Combustible, const float FullMass, const float BurnRate, const float Dt)
{
  float NewMass = Combustible.Mass - BurnRate * Dt;
  UpdateMass(Combustible, NewMass);
  ICombustionInterface::Execute_BurnLoop(Combustible.Object, NewMass / FullMass, Combustible.Index);

  if (NewMass <= 0)
  {
   // Combustibles.Remove(Combustible);
    ICombustionInterface::Execute_BurnCompleted(Combustible.Object, Combustible.Index);
    return true;
  }
  return false;
}

void ACombustionManager::BurnTick()
{
  float Dt = UHeatLibrary::ThermalDt(this);
  TSet<FCombustible> CombustiblesToRemove;
  TArray<FCombustible> CombustiblesArray = Combustibles.Array();
  
  for (auto& Combustible : CombustiblesArray)
  {
    if (!IsValid(Combustible.Object) || !UItemDataSubsystem::Get(this).ItemHasProperty<FItemCombustion>(Combustible.Object)) //This is validity check. somehow pointer isn't nulled after being destroyed
    {
      CombustiblesToRemove.Add(Combustible);
      continue;
    }
    FItemCombustion CombustionData = UItemDataSubsystem::Get(this).GetItemProperty<FItemCombustion>(Combustible.Object);

    DealBurnToSurrounding(Combustible, CombustionData.EnergyDensity, CombustionData.BurnRate, Dt);
    bool bBurnt = SelfCombustion(Combustible, CombustionData.CombustibleMass, CombustionData.BurnRate, Dt);
    if (bBurnt) CombustiblesToRemove.Add(Combustible);
  }

  Combustibles =  Combustibles.Difference(CombustiblesToRemove);

  if (Combustibles.IsEmpty())
  {
    GetWorld()->GetTimerManager().PauseTimer(BurnTimer);
  }
  
}
