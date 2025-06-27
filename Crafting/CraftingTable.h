// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "../Messaging/PayloadStructs.h"
#include "../Messaging/ChannelTags.h"
#include "CraftingTable.generated.h"


UCLASS(Blueprintable, BlueprintType)
class THEGAME_API ACraftingTable : public ABlockActor
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void BindToGizmo();

	void OnSubmitOrCancelCraft(FGameplayTag Channel, const FPayloadKeyIntMap& Payload);

	UFUNCTION(BlueprintCallable)
	void TryCollectDelivery();

	void UnbindFromGizmo();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnCraft();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void DeliveryCollected();

protected:
	UFUNCTION(BlueprintCallable)
	bool IsThereDelivery() const;


private:
	UPROPERTY(SaveGame)
	TMap<FName, int32> Items;
	UPROPERTY()
	FGameplayMessageListenerHandle Handle;
};
