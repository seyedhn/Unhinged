// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Save/SaveGameInterface.h"
#include "../../Messaging/ChannelTags.h"
#include "../../Messaging/PayloadStructs.h"
#include "HallucinationPDA.h"
#include "HallucinationComp.generated.h"



UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API UHallucinationComp : public UActorComponent, public ISaveGameInterface
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	void ProcessHallucination(FGameplayTag Channel, const FPayloadKey& Payload);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsIllusionValid(const FName Key);

	UFUNCTION(BlueprintCallable)
	void ActivateInsanity(bool Insanity);
	void RandomlyPerformInsanity();
	void Hallucinate();
	void SeeIllusion();
	void SkyChange();


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMesh* IllusionMesh;

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UHallucinationPDA* DA;

private:

	UPROPERTY(SaveGame)
	TSet<FName> PlayedHallucinationKeys;

	UPROPERTY(SaveGame)
	TSet<FName> PlayedIllusionKeys;

	FTimerHandle InsanityTimer;
	float Sanity;
};
