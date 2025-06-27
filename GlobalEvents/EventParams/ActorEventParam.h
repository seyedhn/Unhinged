#pragma once

#include "CoreMinimal.h"
#include "ActorEventParam.generated.h"

USTRUCT(Blueprintable)
struct FActorEventParam
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	AActor* Actor = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	int Amount = 0;

};