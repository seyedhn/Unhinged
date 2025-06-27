// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Messaging/ChannelTags.h"
#include "../Messaging/PayloadStructs.h"
#include "Components/AudioComponent.h"
#include "RecallComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRecallDelegate);

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API URecallComp : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	void RecordTimestamps();
	void ResizeTimestampArray();
	FVector ComputeAverageLocation(FVector loc1, FVector loc2);

	UFUNCTION(Blueprintcallable)
	void BeginRecall();
	void ExecuteRecall();
	void EndRecall();

	void SetStampTimer();
	void ClearTimestamps(FGameplayTag Channel, const FPayloadEmpty& Payload);



	UPROPERTY(BlueprintAssignable)
	FRecallDelegate OnRecallCompleted;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* RecallSound;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* RecallWhooshSound;
	
private:

	static const uint16 StampLength = 256;
	uint16 StampIndex = 0;
	FTimerHandle StampTimer;
	FTimerHandle RecallTimer;
	float TimerRate = 0.1;
	TStaticArray<FVector, StampLength> Timestamps;
	UAudioComponent* RecallAudioComp;
};
