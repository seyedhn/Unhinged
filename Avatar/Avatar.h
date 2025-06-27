// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Save/SaveGameInterface.h"
#include "../Equipment/EquipmentHandler.h"
#include "MovementComp.h"
#include "AvatarUI.h"
#include "Hallucination/HallucinationComp.h"
#include "RecallComp.h"
#include "VitalsComp.h"
#include "../Resource/StorageComp.h"
#include "../Postprocess/FPCameraComp.h"
#include "../Audio/VoiceOverComp.h"
#include "Avatar.generated.h"



UCLASS(Blueprintable)
class THEGAME_API AAvatar : public ACharacter //, public ISaveGameInterface
{
	GENERATED_BODY()

public:

	AAvatar();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
	void EnableCharacterInput(bool Enable, bool AffectLookInput);

	void OverrideHealth(float Health, bool Invincible);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UEquipmentHandler* EquipmentHandler;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMovementComp* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAvatarUI* AvatarUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UVoiceOverComp* VoiceOverComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStorageComp* Backpack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UVitalsComp* VitalsComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UFPCameraComp* FPCamera;

protected:
//	FGuid ActorID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URecallComp* RecallComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UHallucinationComp* HallucinationComp;


};
