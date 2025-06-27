// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/OurCommonActivatableWidgetBase.h"
#include "../Messaging/PayloadStructs.h"
#include "CraftingList.generated.h"


UCLASS()
class THEGAME_API UCraftingList : public UOurCommonActivatableWidgetBase
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintCallable)
	void AddItemToMap(FName ItemKey, UWidget* Widget);

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetListItems() const;

	FPayloadKeyIntMap GetCraftedItems();

	UFUNCTION(BlueprintCallable)
	void SubmitCraft();

	UFUNCTION(BlueprintCallable)
	void CancelCraft();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UWidget* IsItemOnList(FName ItemKey, bool& Valid, int32& Amount);

	UFUNCTION(BlueprintCallable)
	void RemoveItemFromMap(FName ItemKey, UWidget* Widget);

	TMultiMap<FName, UWidget*> CraftItems;

};