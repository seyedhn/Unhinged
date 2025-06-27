// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Block/BlockSocket.h"
#include "PipeInterface.generated.h"



UINTERFACE(MinimalAPI)
class UPipeInterface : public UInterface
{
	GENERATED_BODY()
};

class THEGAME_API IPipeInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Pipe)
	void ObjectFlows(FName Object, ABlockActor* PreviousBlock, FSocketData SocketData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Pipe)
	void SelfFlow(FName Object);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Pipe)
	void NetworkEstablished(bool Connected);

	/*
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Pipe)
	FName GetFluidType() const;
	*/
};