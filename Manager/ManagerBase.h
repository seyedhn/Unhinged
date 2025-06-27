// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ManagerBase.generated.h"


UCLASS(Abstract)
class THEGAME_API AManagerBase : public AActor
{
	GENERATED_BODY()

	friend class USaveGameSubsystem;

protected:
	virtual void SaveManager() {};
	virtual void PreLoadManager(bool bRewindLoad) {};
	virtual void LoadManager(bool bRewindLoad) {};

};

