#pragma once

#include "CoreMinimal.h"
#include "EventType_DA.h"
#include "EventParam.h"
#include "EventData.generated.h"

USTRUCT(Blueprintable)
struct FEventData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	UEventType_DA* EventType = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	UDataAsset* EventSpecifier = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FEventParam EventParam;
};