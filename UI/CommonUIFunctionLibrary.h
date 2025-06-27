// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreMinimal.h"
#include "Components/Image.h"
#include "CommonUIFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class THEGAME_API UCommonUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UILibrary")
	static void EnableWASD_Navigation(bool Enable);

	UFUNCTION(BlueprintCallable, Category = "UILibrary")
	static void EnableTabNavigation(bool Enable);

	UFUNCTION(BlueprintCallable, Category = "UILibrary")
	static void InvertColour(UImage* Image);

	UFUNCTION(BlueprintCallable, Category = "UILibrary")
	static void MakeIconBlack(UImage* Image);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UILibrary")
	static FString DoubleDigit(int32 Time);

//	UFUNCTION(BlueprintCallable, Category = "CommonUI")
//	static void EnableMouseWheelNavigation(bool Reset);

};