// Copyright 2024 Seyed Nasrollahi. All Rights Reserved.

#include "DamageLibrary.h"
#include "../Manager/ManagerSubsystem.h"

void UDamageLibrary::DealDamage(const UObject* WorldContext, AActor* HitActor, float Damage, EDamageType DamageType)
{
  UManagerSubsystem::Get(WorldContext).DamageManager->DealHitDamage(FHitDamageStruct(HitActor, nullptr, Damage, DamageType, 0, FVector(0.f), FVector(0.f), NAME_None));
}

void UDamageLibrary::DealHitDamage(const UObject* WorldContext, AActor* HitActor, UPrimitiveComponent* HitComp, float Damage, EDamageType DamageType, int32 Index, FVector HitLocation, FVector HitNormal, FName HitBone)
{
  UManagerSubsystem::Get(WorldContext).DamageManager->DealHitDamage(FHitDamageStruct(HitActor, HitComp, Damage, DamageType, Index, HitLocation, HitNormal, HitBone));
}

void UDamageLibrary::DealRadialDamage(const UObject* WorldContext, UPrimitiveComponent* HitComp, const float Radius, const float Damage, const float Impulse, const EDamageType DamageType)
{
  UManagerSubsystem::Get(WorldContext).DamageManager->ProcessRadialDamage(FRadialDamageStruct(HitComp, Radius, Damage, Impulse, DamageType));
}

void UDamageLibrary::DealOverlapDamage(const UObject* WorldContext, UPrimitiveComponent* HitComp, float Damage, EDamageType DamageType, int32 Index)
{
  UManagerSubsystem::Get(WorldContext).DamageManager->ProcessOverlapDamage(FDamageStruct(nullptr, HitComp, Damage, DamageType, Index));
}
