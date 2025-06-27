// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Avatar/Hallucination/HallucinationPDA.h"
#include "MediaSource.h"
#include "AvatarUI.generated.h"

/**
 * 
 */
UCLASS()
class THEGAME_API UAvatarUI : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ActivateCrosshair(bool bActivate);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnDeath();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnRecall();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnWakeupInBed();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnFullDeath();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnSleep(int32 Hours);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ScreenDamageEffect(float Health);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ActivateWeaponWheel(bool bActivate);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlayHallucination(FHallucination HallucinationData);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void HideAvatarUI(const bool Hide);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Recall;
};