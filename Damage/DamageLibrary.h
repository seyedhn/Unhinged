// Copyright 2024 Seyed Nasrollahi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DamageManager.h"
#include  "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "DamageLibrary.generated.h"


UCLASS(Blueprintable)
class THEGAME_API UDamageLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void DealDamage(const UObject* WorldContextObject, AActor* HitActor, float Damage, EDamageType DamageType);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void DealHitDamage(const UObject* WorldContextObject, AActor* HitActor, UPrimitiveComponent* HitComp, float Damage, EDamageType DamageType, int32 Index, FVector HitLocation, FVector HitNormal, FName HitBone);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void DealRadialDamage(const UObject* WorldContextObject, UPrimitiveComponent* HitComp, const float Radius, const float Damage, const float Impulse, const EDamageType DamageType);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void DealOverlapDamage(const UObject* WorldContextObject, UPrimitiveComponent* HitComp, float Damage, EDamageType DamageType, int32 Index);

};