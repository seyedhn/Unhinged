// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ItemsData/ItemPropertyStructs.h"
#include "../Messaging/ChannelTags.h"
#include "../Messaging/PayloadStructs.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "TrialStart.generated.h"

UENUM(BlueprintType)
enum class ETrialState : uint8
{
	Unavailable	 UMETA(DisplayName = "Unavailable"),
	Available  UMETA(DisplayName = "Available"),
	Active  UMETA(DisplayName = "Active"),
	Complete UMETA(DisplayName = "Complete")
};


UCLASS(Blueprintable, BlueprintType)
class THEGAME_API ATrialStart : public AGameActor
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void BindToGizmo();
	void UnbindFromGizmo();
	void OnTrialAcceptedOrDeclined(FGameplayTag Channel, const FPayloadBool& Payload);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void StartTrial();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Default)
	FName Key;

protected:

	UPROPERTY(BlueprintReadWrite)
	float Counter;
		
	UPROPERTY(BlueprintReadWrite)
	FTimerHandle Timer;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	float TimeRecord;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	int32 CurrentStage;

	UPROPERTY(BlueprintReadWrite)
	FTrialData TrialData;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float CounterInterval;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	ETrialState TrialState;



private:

	FGameplayMessageListenerHandle Handle;

};
