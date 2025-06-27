// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStaminaDelegate, float, Stamina);

UENUM(BlueprintType)
enum class EAvatarMotion : uint8
{
	Idle	 UMETA(DisplayName = "Idle"),
	Walk  UMETA(DisplayName = "Walk"),
	Run  UMETA(DisplayName = "Run"),
	Crouch UMETA(DisplayName = "Crouch")
};


UCLASS(Blueprintable, BlueprintType)
class THEGAME_API UMovementComp : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateHeadBobbing(EAvatarMotion NewMotion);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void StopHeadBobbing();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void DrainStamina(float DrainPoints);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void GainStamina();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void LimitLook(bool ApplyLimit);

	UFUNCTION(BlueprintCallable)
	void FullyDrainStamina();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Stamina;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EAvatarMotion Motion;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bSprinting;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bEnableHeadBobbing;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class AAvatar* Avatar;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle StaminaTimer;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStaminaDelegate OnStaminaUpdated;


};
