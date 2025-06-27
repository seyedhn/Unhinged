// Copyright 2024 Seyed Nasrollahi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include  "Kismet/BlueprintFunctionLibrary.h"
#include "ResourceManager.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "ResourceLibrary.generated.h"

class UStorageComp;

UCLASS(Blueprintable)
class THEGAME_API UResourceLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void SpawnItem(const UObject* WorldContextObject, const FName Key, const FVector Location, const FVector Impulse);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void SpawnItemWithAmount(const UObject* WorldContextObject, const FName Key, const int32 Amount, const FVector Location, const FVector Impulse);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static int32 GetNumItemsCraftable(const UObject* WorldContextObject, const FName Key);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static int32 GetItemAmount(const UObject* WorldContextObject, const FName Key);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void DeductFromStorage(const UObject* WorldContextObject, UStorageComp* Storage, FName Key, int32 Amount);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void DeductFromBackpack(const UObject* WorldContextObject, FName Key, int32 Amount);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void ConsumeFromBackpack(const UObject* WorldContextObject, FName Key);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryDeductFromBackpack(const UObject* WorldContextObject, FName Key, int32 Amount);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryAddToStorage(const UObject* WorldContextObject, UStorageComp* Storage, FName Key, int32 Amount);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryCollectItem(const UObject* WorldContextObject, FName Key, int32 Amount);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryCollectItems(const UObject* WorldContextObject, const TArray<FName>& Items, TArray<FName>& RemainingItems);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryTransferItem(const UObject* WorldContextObject, UStorageComp* From, UStorageComp* To, FName Key, int32 Amount);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", ReturnDisplayName = "Successful"))
	static bool TryCollectDelivery(const UObject* WorldContextObject, const FName Item, const int32 Amount);
};