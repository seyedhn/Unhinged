// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RewindComp.h"
#include "../Manager/ManagerBase.h"
#include "../Save/SaveGameInterface.h"
#include "Sound/SoundMix.h"
#include "Components/AudioComponent.h"
#include "RewindManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRewindEndDelegate);


UCLASS(Blueprintable, BlueprintType)
class THEGAME_API ARewindManager : public AManagerBase, public ISaveGameInterface
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Rewind")
	TArray<URewindComp*> GetActiveRewindComps() const;

	UFUNCTION(BlueprintCallable, Category = "Rewind", meta = (ReturnDisplayName = "Successful"))
	bool BeginFullRewind();

	UFUNCTION(BlueprintCallable, Category = "Rewind")
	void OnRewindBegin(URewindComp* RewindComp);

	UFUNCTION(BlueprintCallable, Category = "Rewind")
	void CancelAllRewinds();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Rewind")
	bool IsRewinding() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Rewind")
	bool IsFullRewinding() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Rewind")
	URewindComp* GetLongestRewindComp() const;

	void OnRewindAdded(URewindComp* RewindComp);
	void OnRewindRemoved(URewindComp* RewindComp);

	void ActivateRewindPP(bool Activate) const;


	UPROPERTY(BlueprintAssignable)
	FRewindEndDelegate OnRewindEnded;

private:
	UFUNCTION()
	void OnRewindEnd(URewindComp* RewindComp);


public:

	//TIME BENDER PROPERTIES//

	UPROPERTY(BlueprintReadWrite, SaveGame, Category = "Rewind")
	int32 TBenderAvailableCapsules = 1;

	UPROPERTY(BlueprintReadWrite, SaveGame, Category = "Rewind")
	int32 TBenderFilledCapsules = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Rewind")
	TSet<URewindComp*> DormantRewindComps;

	UPROPERTY(BlueprintReadWrite, Category = "Rewind")
	TArray<URewindComp*> ActiveRewindComps;

private:
	UPROPERTY()
	USoundBase* RewindSound;
	UPROPERTY()
	USoundBase* RewindEndSound;
	UPROPERTY()
	USoundMix* RewindSoundMix;
	UPROPERTY()
	UAudioComponent* RewindSoundComp;
	UPROPERTY()
	bool bFullRewind;

};
