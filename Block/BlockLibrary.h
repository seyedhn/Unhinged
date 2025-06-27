// Copyright 2024 Seyed Nasrollahi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BlockSMC.h"
#include  "Kismet/BlueprintFunctionLibrary.h"
#include "BlockLibrary.generated.h"


UCLASS(Blueprintable)
class THEGAME_API UBlockLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void ConnectSockets(FSocketData socket1, FSocketData socket2);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static int32 GetSocketInOutDirection(FSocketData socket);
};