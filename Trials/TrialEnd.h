// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ItemsData/ItemPropertyStructs.h"
#include "../Messaging/ChannelTags.h"
#include "../Messaging/PayloadStructs.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "TrialEnd.generated.h"


UCLASS(Blueprintable, BlueprintType)
class THEGAME_API ATrialEnd : public AGameActor
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void BindToGizmo();
	void UnbindFromGizmo();
	void OnRewardSelected(FGameplayTag Channel, const FPayloadKeyInt& Payload);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnTrialStarted(int32 RewardTier);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnTrialCancelled();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void DeliverReward();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void RewardDelivered();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void DeliveryCollected();

	UFUNCTION(BlueprintCallable)
	void TryCollectDelivery();




	UPROPERTY(BlueprintReadWrite, Category=Default)
	FName Key;

protected:

private:
	UPROPERTY(SaveGame)
	FName RewardKey;
	UPROPERTY(SaveGame)
	int32 RewardAmount;

	FGameplayMessageListenerHandle Handle;

};
