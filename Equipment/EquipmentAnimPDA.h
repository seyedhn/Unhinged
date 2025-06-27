// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EquipmentAnimPDA.generated.h"


UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	NoAttack  UMETA(DisplayName = "NoAttack"),
	HoldToAttack	 UMETA(DisplayName = "HoldToAttack"),
	PressToAttack  UMETA(DisplayName = "PressToAttack"),
};

UENUM(BlueprintType)
enum class EEquipmentAnimState : uint8
{
	None  UMETA(DisplayName = "None"),
	Equip  UMETA(DisplayName = "Equip"),
	Unequip  UMETA(DisplayName = "Unequip"),
	Attack  UMETA(DisplayName = "Attack")
};


UCLASS(Blueprintable)
class THEGAME_API UEquipmentAnimPDA : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName AttachSocket;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	EEquipmentType Type;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UAnimInstance> AnimLayer;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TMap<EEquipmentAnimState, UAnimMontage*> AnimMontages;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	USoundBase* EquipSound;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	USoundBase* UnequipSound;

private:



};
