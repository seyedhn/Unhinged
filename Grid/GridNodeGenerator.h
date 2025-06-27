// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridNode.h"
#include "GridNodeGenerator.generated.h"




UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API UGridNodeGenerator : public UGridNode
{
	GENERATED_BODY()


public:

	//INITIALISATION

	virtual void BeginPlay() override;

	virtual void OnMaterialize() override;

	virtual void OnDestroyed(AActor* Actor) override;

	virtual void PropagateNetwork(UGridNode* NodeBefore) override;

	virtual bool CanTakeConnection() const override;


	//CONNECT TO GRID//

	virtual void ConnectToGrid(bool ForceGridReset = false) override;

private:

	void MakeGrid();
	void InitiatePropagationOnLevelLoad();


};
