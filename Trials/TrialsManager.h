// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Manager/ManagerBase.h"
#include "../Messaging/PayloadStructs.h"
#include "../Messaging/ChannelTags.h"
#include "TrialStart.h"
#include "TrialsManager.generated.h"


UCLASS(Blueprintable, BlueprintType)
class THEGAME_API ATrialsManager : public AManagerBase
{
	GENERATED_BODY()

public:

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 CurrentTier;

private:

};
