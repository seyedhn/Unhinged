// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FuseInterface.generated.h"



UINTERFACE(MinimalAPI)
class UFuseInterface : public UInterface
{
	GENERATED_BODY()
};

class THEGAME_API IFuseInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Fuse)
	void DestroyFusedObject();

	UFUNCTION(BlueprintNativeEvent, BLueprintCallable, Category = Fuse)
	void TransformFuseObject(FName FusedItem, int32 Amount);

};