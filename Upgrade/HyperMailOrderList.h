// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/OurCommonActivatableWidgetBase.h"
#include "../Messaging/PayloadStructs.h"
#include "HyperMailOrderList.generated.h"


UCLASS()
class THEGAME_API UHyperMailOrderList : public UOurCommonActivatableWidgetBase
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintCallable)
	void AddItemToMap(FName ItemKey, UWidget* Widget);

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetOrderListItems() const;

	FPayloadHyperMail GetOrderedUpgradesAndDeliveredItems();

	UFUNCTION(BlueprintCallable)
	void SubmitOrder();

	UFUNCTION(BlueprintCallable)
	void CancelOrder();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UWidget* IsItemOnList(FName ItemKey, bool& Valid, int32& Amount);

	UFUNCTION(BlueprintCallable)
	void RemoveItemFromMap(FName ItemKey, UWidget* Widget);

	TMultiMap<FName, UWidget*> OrderItems;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 TotalCost;

	int32 Cashback = 0;

};