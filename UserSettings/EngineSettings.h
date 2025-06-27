// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "EngineSettings.generated.h"


UCLASS(Config=Engine)
class THEGAME_API UEngineSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	void SetDefaultRHI(FString RHI);
};
