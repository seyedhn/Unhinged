#pragma once

#include "CoreMinimal.h"
#include "MachineEventParam.generated.h"

USTRUCT(Blueprintable)
struct FMachineEventParam
{
    GENERATED_USTRUCT_BODY();

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
    bool LoadSavedMachine = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
        FString PresavedMachineName;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
        FVector Location = FVector(0.f);

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
        bool DestroyCurrentMachine = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
        bool SaveMachineData = false;

};