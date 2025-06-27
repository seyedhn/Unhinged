#pragma once

#include "CoreMinimal.h"
#include "EventParams/ActorEventParam.h"
#include "EventParams/MachineEventParam.h"
#include "EventParams/MaterialEventParam.h"
#include "GameFramework/SaveGame.h"
#include "EventParam.generated.h"


UENUM(Blueprintable)
enum class EEventParamType : uint8
{
	Actor UMETA(DisplayName = "ActorParam"),
	Machine UMETA(DisplayName = "MachineParam"),
	Material UMETA(DisplayName = "MaterialParam"),
	SaveGame UMETA(DisplayName = "SaveGameParam"),
};

USTRUCT(Blueprintable)
struct FEventParam
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	EEventParamType Type = EEventParamType::Actor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables, meta = (EditCondition = "Type==EEventParamType::Actor"))
	FActorEventParam ActorParam;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables, meta = (EditCondition = "Type==EEventParamType::Machine"))
	FMachineEventParam Machine;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables, meta = (EditCondition = "Type==EEventParamType::Material"))
	FMaterialEventParam Material;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables, meta = (EditCondition = "Type==EEventParamType::SaveGame"))
	USaveGame* SaveGame = nullptr;

};