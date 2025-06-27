// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionComp.generated.h"

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	NonInteractable  UMETA(DisplayName = "NonInteractable"),
	Interactable	 UMETA(DisplayName = "Interactable"),
	Progress  UMETA(DisplayName = "Progress"),
	ItemSelection UMETA(DisplayName = "ItemSelection")
};

UENUM(BlueprintType)
enum class EInteractionStatus : uint8
{
	Started	 UMETA(DisplayName = "Started"),
	Stopped  UMETA(DisplayName = "Stopped"),
	Completed  UMETA(DisplayName = "Completed")
};

USTRUCT(Blueprintable)
struct FInteraction
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	EInteractionType InteractionType = EInteractionType::Interactable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FString DescriptionBeforeName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FString DescriptionAfterName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	bool IncludeNameInDescription = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float ProgressInteractLength = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	bool DisableAfterProgress = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float DisableTimer = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	bool CheckForDescriptionUpdate = false;
};

USTRUCT(Blueprintable)
struct FResourceFill
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FName ResourceName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float ResourceUnitFillTime = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	int32 ResourceMaxLimit = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	int32 AmountToAddAtEveryIteration = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	bool ResourceCanBeRemoved = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	USoundBase* ResourceFillSound = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	USoundBase* ResourceRemoveSound = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	bool PlaySoundOnlyOnceAtStart = false;
};

UCLASS(Blueprintable, BlueprintType)
class THEGAME_API UInteractionComp : public UActorComponent
{
	GENERATED_BODY()

public:


};
