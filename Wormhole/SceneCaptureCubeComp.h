// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Messaging/PayloadStructs.h"
#include "../Messaging/ChannelTags.h"
#include "Components/SceneCaptureComponentCube.h"
#include "SceneCaptureCubeComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerIsClose, USceneCaptureCubeComp*, SceneCapture, bool, bPlayerIsClose);

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API USceneCaptureCubeComp : public USceneCaptureComponentCube
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SoftInitialise();

	UFUNCTION(BlueprintCallable)
	void Initialise(USceneCaptureCubeComp* Other, float ActiveDistance);

	UFUNCTION(BlueprintCallable)
	void Deinitialise();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsPlayerClose() const;

	UFUNCTION(BlueprintCallable)
	void CheckIfPlayerIsClose();

	UFUNCTION(BlueprintCallable)
	void Capture();

	UFUNCTION(BlueprintCallable)
	void TryCapture();

	UFUNCTION(BlueprintCallable)
	void MarkSceneDirty();
	
	UFUNCTION(BlueprintCallable)
	void MarkSceneClean();

	UFUNCTION()
	void ListenForCaptures(FGameplayTag Channel, const FPayloadEmpty& Payload);

	void RotateTowardsPlayer();

	void CheckIfTextureIsGreen();

	USceneCaptureCubeComp* OtherSceneCapture;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FPlayerIsClose OnPlayerIsClose;

	bool bIsSceneDirty = true;
	bool bPlayerIsClose = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ActivationDistance;

protected:

	FTimerHandle CaptureTimer;
	FTimerHandle ActiveTimer;
	FTimerHandle CheckPlayer;


};
