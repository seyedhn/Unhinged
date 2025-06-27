// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "../Save/SaveGameInterface.h"
#include "StorageComp.generated.h"


UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API UStorageComp : public UActorComponent, public ISaveGameInterface
{
	GENERATED_BODY()

public:

	void AddToStorage(const FName Item, const int32 Amount);
	void DeductFromStorage(const FName Item, const int32 Amount);
	void UpdateStorage(const FName Item, const int32 Amount);

	UFUNCTION(BlueprintCallable)
	int32 GetItemAmount(const FName Item) const;

	UFUNCTION(BlueprintCallable)
	int32 GetItemStack(const FName Item) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetStorageCapacity() const;

	UFUNCTION(BlueprintCallable)
	void IncreaseStorageCapacity(int32 Amount);

	UFUNCTION(BlueprintCallable)
	void SetStorageCapacity(int32 Amount);

	UFUNCTION(BlueprintCallable)
	int32 GetNumOccupiedSlots() const;

	UFUNCTION(BlueprintCallable)
	int32 GetNumFreeSlots() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPARAM(DisplayName = "Keys")
	TArray<FName> GetStackedSlots(TArray<int32>& Amounts) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPARAM(DisplayName = "Keys")
	int32 GetNumItemsCraftable(const FName Key) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPARAM(DisplayName = "Keys")
	TArray<FName> GetItemsWithProperty(const UScriptStruct* PropertyType, TArray<int32>& Amounts) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPARAM(DisplayName = "Keys")
	TArray<FName> GetItemsWithTag(const FGameplayTag& Tag, TArray<int32>& Amounts) const;

	UFUNCTION(BlueprintCallable)
	UPARAM(DisplayName = "Successful")
	bool TryAddItem(FName Key, int32 Amount);

	UFUNCTION(BlueprintCallable)
	UPARAM(DisplayName = "Successful")
	bool TryDeductItem(FName Key, int32 Amount);

	UFUNCTION(BlueprintCallable)
	void DropStorage();

private:
	UPROPERTY(EditDefaultsOnly, SaveGame)
	int32 Capacity;

	UPROPERTY(SaveGame)
	TMap<FName, int32> Storage;
};
