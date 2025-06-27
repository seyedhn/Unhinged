// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceChargeSMC.generated.h"

UENUM(BlueprintType)
enum class ESpaceChargeType : uint8
{
	Free	 UMETA(DisplayName = "Free"),
	Pole  UMETA(DisplayName = "Pole"),
	Central  UMETA(DisplayName = "Central"),
};


UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API USpaceChargeSMC : public UStaticMeshComponent
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable)
	bool IsConnectionValid(USpaceChargeSMC* other);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ESpaceChargeType ChargeType;

protected:


private:


};
