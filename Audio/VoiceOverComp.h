// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "VoiceOverPDA.h"
#include "../Messaging/ChannelTags.h"
#include "../Messaging/PayloadStructs.h"
#include "VoiceOverComp.generated.h"



UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API UVoiceOverComp : public UAudioComponent, public ISaveGameInterface
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	UFUNCTION()
	void PlayVO(FGameplayTag Channel, const FPayloadKey& Payload);
	UFUNCTION()
	void OnVoiceOverFinished();
	UFUNCTION(BlueprintCallable)
	void PlayRecallVO();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UVoiceOverPDA* VoiceOverDA;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* RecallVO;

	UPROPERTY(BlueprintReadOnly, SaveGame)
	TSet<FName> PlayedVOs;

protected:

	UPROPERTY()
	FName CurrentKey;


};
