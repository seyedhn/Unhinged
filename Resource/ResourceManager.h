// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Manager/ManagerBase.h"
#include "ResourceContainer.h"
#include "StorageComp.h"
#include "ResourceManager.generated.h"


UENUM(BlueprintType)
enum class EInventoryType : uint8
{
	Backpack  UMETA(DisplayName = "Backpack"),
	Storage  UMETA(DisplayName = "Storage"),
};



UCLASS(Blueprintable, BlueprintType)
class THEGAME_API AResourceManager : public AManagerBase
{
	GENERATED_BODY()

public:
	AResourceManager();
	void BeginPlay() override;
	void SetResourceContainer();
	void SetBackpackData();

	UPROPERTY(BlueprintReadOnly)
	AResourceContainer* ResourceContainer;

	UPROPERTY(BlueprintReadOnly)
	UStorageComp* CentralStorage;

	UPROPERTY(BlueprintReadOnly)
	UStorageComp* Backpack;


};
