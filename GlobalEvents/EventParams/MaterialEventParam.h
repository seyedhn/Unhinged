#pragma once

#include "CoreMinimal.h"
#include "MaterialEventParam.generated.h"

USTRUCT(Blueprintable)
struct FMaterialEventParam
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
		FString MaterialSymbol;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
		int Amount = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
		bool Infinite = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
		bool IsOre = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
		bool UnlockMineral = false;

};
