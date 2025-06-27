// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "../Messaging/PayloadStructs.h"
#include "../Messaging/ChannelTags.h"
#include "../Block/BlockActor.h"
#include "HyperMail.generated.h"


UCLASS(Blueprintable, BlueprintType)
class THEGAME_API AHyperMail : public ABlockActor
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void BindToDevice();

	void OnSubmitOrCancelOrder(FGameplayTag Channel, const FPayloadHyperMail& Payload);

	UFUNCTION(BlueprintCallable)
	void TryCollectDelivery();

	void UnbindFromDevice();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnOrderPlaced();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void DeliveryCollected();

	UFUNCTION(BlueprintCallable, meta = (HideSelfPin, DefaultToSelf))
	bool IsThereDelivery() const;

	virtual void Serialize(FArchive& Ar) override;


protected:


private:
	UPROPERTY(SaveGame)
	TMap<FName, int32> Consumables;
	UPROPERTY(SaveGame)
	TArray<FName> Upgrades;
	UPROPERTY()
	FGameplayMessageListenerHandle Handle;
};
