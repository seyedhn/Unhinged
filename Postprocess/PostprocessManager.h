// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Manager/ManagerBase.h"
#include "DynamicPPComp.h"
#include "PostprocessPDA.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "PostprocessManager.generated.h"






UCLASS(Blueprintable, BlueprintType)
class THEGAME_API APostprocessManager : public AManagerBase
{
	GENERATED_BODY()

public:

	//UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "PPMaterial")
	//void RemovePP(UDynamicPPComp Comp);

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	APostProcessVolume* PPVolume;

protected:

	/*
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, UDynamicPPComp*> DynamicPPs;

	UPROPERTY(BlueprintReadOnly)
	UPostprocessPDA* PPDA;
	*/

//	UFUNCTION(BlueprintCallable)
//	UDynamicPP* CreateDynamicPP(class UMaterialInterface* Parent, FName OptionalName, EMIDCreationFlags CreationFlags);

	//UFUNCTION(BlueprintCallable)
//	UDynamicPP* CreateDynamicPP(class UMaterialInterface* Parent, FName OptionalName, EMIDCreationFlags CreationFlags);
};
