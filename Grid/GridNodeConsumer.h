// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridNode.h"
#include "GridNodeConsumer.generated.h"




UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API UGridNodeConsumer : public UGridNode
{
	GENERATED_BODY()


public:

	//INITIALISATION

	virtual void PropagateNetwork(UGridNode* NodeBefore) override;

	virtual void OnMaterialize() override;

	virtual void OnDestroyed(AActor* Actor) override;

	virtual bool CanTakeConnection() const override;

	//CONNECT TO GRID//

	virtual void ConnectToGrid(bool ForceGridReset = false) override;

	//A power of 0.0 resets it back to the idle power
	UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool TryUpdatePower(float NewPower);

protected:

};
