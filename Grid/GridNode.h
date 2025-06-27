// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridStruct.h"
#include "GridNode.generated.h"


UCLASS(Abstract)
class THEGAME_API UGridNode : public UStaticMeshComponent
{
	GENERATED_BODY()

public:

	friend class AGridManager;

	UGridNode();
	virtual void PropagateNetwork(UGridNode* NodeBefore) {};
	void RemoveNeighbour(UGridNode* Other);

protected:


	//Need codes to support: Begin level (DONE), adding nodes (DONE), removing nodes (DONE), Load level (DONE), rewind load (DONE)
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnMaterialize() {};
	UFUNCTION()
	virtual void OnDestroyed(AActor* Actor) {};

	virtual void ConnectToGrid(bool ForceGridReset = false) {};
	UNiagaraComponent* MakeTether(UGridNode* Other);
	void UpdateTetherColour(bool bActive);

	TArray<UGridNode*> FindNearbyNodes();
	UGridNode* FindNearestNode();
	UGridNode* FindNearestFreeNode();
	void SortNodesByDistance(TArray<UGridNode*>& Nodes);
	void AddNeighbour(UGridNode* Other);
	void PlayPowerSound(bool Restore);

public:

	void OnPowerRestore();
	void OnPowerOutage();
	void Reset();

	UFUNCTION(BlueprintCallable)
	bool IsConnectedToGrid() const;
	UFUNCTION(BlueprintCallable)
	bool IsPowered() const;

	virtual bool CanTakeConnection() const { return false; };
	FGuid GetGridID() const;
	UGridNode* GetNeighbour() const;


protected:

	FGuid GridID;
	TArray<FGridConnection> Connections;
	bool bIsPowered;
	UAudioComponent* PowerSound;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float IdlePower;

	UPROPERTY(BlueprintReadOnly)
	float Power;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float RangeRadius = 2500.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USoundBase* RestoreSound;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USoundBase* OutageSound;

};
