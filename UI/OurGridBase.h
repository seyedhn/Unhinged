// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OurCommonActivatableWidgetBase.h"
#include "OurGridBase.generated.h"

class UOurButtonBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FItemSelectedDelegate, FName, ItemKey, int32, ButtonIndex, int32, GridIndex);
/**
 * 
 */
UCLASS()
class THEGAME_API UOurGridBase : public UOurCommonActivatableWidgetBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid", meta = (ExposeOnSpawn = true))
	FText HeaderText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid", meta = (ExposeOnSpawn = true))
	int32 Rows;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid", meta = (ExposeOnSpawn = true))
	int32 Columns;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid", meta = (ExposeOnSpawn = true))
	int32 GridIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Grid")
	TSet<FName> CurrentShowingItemNames;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid", meta = (ExposeOnSpawn = true))
	FString NumericalSymbol;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid", meta = (ExposeOnSpawn = true))
	bool bUseLargeIcons;

	UPROPERTY(BlueprintAssignable)
	FItemSelectedDelegate OnItemSelected;

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Grid")
	TArray<UOurButtonBase*> Buttons;


};