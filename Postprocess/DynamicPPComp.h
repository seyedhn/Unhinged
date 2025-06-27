// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PostprocessPDA.h"
#include "PPComp.h"
#include "Components/TimelineComponent.h"
#include "DynamicPPComp.generated.h"

class UFPCameraComp;

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API UDynamicPPComp : public UPPComp
{
	GENERATED_BODY()

public:

//	UDynamicPPComp();

	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* InitialisePPMaterial(FName Key, FPostprocessStruct PPStruct, UFPCameraComp* Camera);
	
	UFUNCTION(BlueprintCallable)
	void InitialisePPSettings(UFPCameraComp* Camera, FPostProcessSettings PPSettings);


	//Timeline//

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ActivatePP();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void DeactivatePP();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void BlendInPPSettings();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void RemovePPSettings();


	UFUNCTION(BlueprintCallable)
	void RemovePP();

	UFUNCTION()
	void OnTimelineUpdated(float alpha);
	UFUNCTION()
	void OnTimelineFinished();

	UPROPERTY()
	UTimelineComponent* PPTimeline;

	//UPROPERTY()
	//FTimeline PPTimeline;

	UPROPERTY()
	UCurveFloat* Curve;

	UPROPERTY(BlueprintReadOnly)
	UMaterialInstanceDynamic* DynamicPP;

protected:

	UPROPERTY(BlueprintReadOnly)
	FName MaterialKey;

	UPROPERTY(BlueprintReadOnly)
	FPostprocessStruct PPStruct;

	UPROPERTY(BlueprintReadOnly)
	UFPCameraComp* FPCamera;

protected:
	

};
