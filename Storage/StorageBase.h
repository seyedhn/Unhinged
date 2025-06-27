// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Block/BlockActor.h"
#include "SpaceChargeSMC.h"
#include "StorageBase.generated.h"



UCLASS(Blueprintable, BlueprintType)
class THEGAME_API AStorageBase : public ABlockActor
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable)
	void AddConnection(AStorageBase* Other, UActorComponent* VFX);

	UFUNCTION(BlueprintCallable)
	void Disconnect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ConnectToCentral(bool Connect);

	virtual void ConnectToCentral_Implementation(bool Connect);

	bool IsConnected();

protected:

	UFUNCTION(BlueprintCallable)
	void PropagateNetwork();


protected:


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Storage")
	bool ConnectedToCentral;

	TArray<TTuple<AStorageBase*, UActorComponent*>> ImmediateConnections;



};
