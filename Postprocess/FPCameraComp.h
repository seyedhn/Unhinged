// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Manager/ManagerBase.h"
#include "../Postprocess/DynamicPPComp.h"
#include "../Postprocess/PostprocessPDA.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "FPCameraComp.generated.h"


UENUM(BlueprintType)
enum class EDetectionType : uint8
{
	None UMETA(DisplayName = "None"),
	Interact	 UMETA(DisplayName = "Interact"),
	Grab  UMETA(DisplayName = "Grab"),
	Deconstruct  UMETA(DisplayName = "Deconstruct")
};



UCLASS(Blueprintable, BlueprintType)
class THEGAME_API UFPCameraComp : public UCameraComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "PPMaterial")
	void RemovePPMaterial(FName MaterialKey, UObject* PPMaterial);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "FPCamera")
	void DepthOfField(bool Enable);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "FPCamera")
	void PulseChromatic(float Strength);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "FPCamera")
	void EnableFisheye(bool Enable);

	UFUNCTION(BlueprintCallable, Category = "FPCamera")
	void SetFoV(const float FoV);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "FPCamera")
	void SetOutlinePP(const bool MSAA);

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, UDynamicPPComp*> DynamicPPs;

	UPROPERTY(BlueprintReadOnly)
	UPostprocessPDA* PPDA;

	UPROPERTY(BlueprintReadWrite)
	float DefaultFoV;

};
