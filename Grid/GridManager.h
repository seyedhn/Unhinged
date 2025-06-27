// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridNode.h"
#include "../Manager/ManagerBase.h"
#include "GridManager.generated.h"



UCLASS(Blueprintable, BlueprintType)
class THEGAME_API AGridManager: public AManagerBase
{
	GENERATED_BODY()


public:

	void AddNodeToGrid(FGuid GridID, UGridNode* Node, float Power, bool ForcePowerCheck = false);
	void RemoveNodeFromGrid(FGuid GridID, UGridNode* Node, float Power);
	void RemoveNodeFromGrid(FGrid* Grid, UGridNode* Node, float Power);
	void RemoveSourceFromGrid(FGuid GridID, UGridNode* Node, float Power);
	void UpdateNodePower(FGuid GridID, float OldPower, float NewPower, bool DelayedPowerCheck);

	void MakeNewGrid(UGridNode* Generator);
	void PropagateGenerators(const TArray<UGridNode*>& Generators);

	void ReEstablishGrid(const FGuid& GridID);
	void ReEstablishGrids(const TArray<FGuid>& GridIDs);

	void ClearGrid(FGrid* Grid);

	UFUNCTION()
	void CheckGridPower(const FGuid& GridID, bool ForceCheck);

protected:

	virtual void LoadManager(bool bRewindLoad) override;

private:
	UPROPERTY()
	TSet<FGrid> Grids;

	FTimerHandle Timer;


};
