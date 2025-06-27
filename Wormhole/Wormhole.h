// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Avatar/Avatar.h"
#include "SceneCaptureCubeComp.h"
#include "Wormhole.generated.h"


UENUM(BlueprintType)
enum class EWormholeType : uint8
{
	Static	 UMETA(DisplayName = "Static"),
	Dynamic  UMETA(DisplayName = "Dynamic"),
};


UCLASS(Blueprintable, BlueprintType)
class THEGAME_API AWormhole : public AGameActor
{
	GENERATED_BODY()

public:

	AWormhole();

	bool SaveAllComps_Implementation() const override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ActivateWormhole(bool Activate);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void LerpMaterial(float Alpha);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ActivateDynamicWormhole(bool Activate, FVector Location);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void FixateWormhole();

	UFUNCTION(BlueprintCallable)
	void WormholeSucked();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn))
	EWormholeType Type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsLive;

protected:

	UPROPERTY(BlueprintReadWrite)
	AAvatar* Avatar;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* WormholeMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USceneCaptureCubeComp* SceneCapture;


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* WormholeMesh2;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USceneCaptureCubeComp* SceneCapture2;

};
