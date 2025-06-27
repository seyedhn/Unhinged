// Copyright 2024 Seyed Nasrollahi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include  "Kismet/BlueprintFunctionLibrary.h"
#include "ItemPropertyStructs.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Upgrade/UpgradeManager.h"
#include "../Upgrade/HyperMail.h"
#include "ItemsDataLibrary.generated.h"


UCLASS(Blueprintable)
class THEGAME_API UItemsDataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", ReturnDisplayName = "Valid", ExpandBoolAsExecs = "ReturnValue"))
	static bool GetItemDrops(const UObject* WorldContextObject, const FName ItemKey, TArray<FName>& Drops);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static EUpgradeType GetItemUpgradeType(const UObject* WorldContextObject, const FName ItemKey);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static EPurchaseType GetItemPurchaseType(const UObject* WorldContextObject, const FItemPurchaseData PurchaseData);

};