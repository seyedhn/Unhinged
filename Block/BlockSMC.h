// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "ItemDataInterface.h"
#include "../Save/SaveGameInterface.h"
#include "../Fuse/FuseInterface.h"
#include "../Grab/GrabInterface.h"
#include "BlockSocket.h"
#include "BlockSMC.generated.h"

UENUM(BlueprintType)
enum class EBlockType : uint8
{
	Static	 UMETA(DisplayName = "Static"),
	Dynamic  UMETA(DisplayName = "Dynamic"),
	Spline  UMETA(DisplayName = "Spline")
};

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API UBlockSMC : public UStaticMeshComponent, public IItemDataInterface, public IFuseInterface, public IGrabInterface, public ISaveGameInterface
{
	GENERATED_BODY()


public:

	UBlockSMC();

	void RemoveConnection(FName socketName);
	void DetachComponent();

	TMap<FName, FSocketData> GetSocketMap() const;
	bool IsSocketOfType(FName SocketName, EBlockSocketType Type) const;

	FName GetItemKey_Implementation() const override;
	bool IsItemActor_Implementation() const override;
	void TransformFuseObject_Implementation(FName FusedItem, int32 Amount) override;
	void GrabObject_Implementation(bool bIsGrabbed) override;
	bool IsObjectGrabbed_Implementation() const override;
	void OnActorReloaded_Implementation(bool RewindLoad) override;

	UFUNCTION(BlueprintCallable)
	void AddNewConnection(FName hostSocket, FSocketData ForeignSocket, UPhysicsConstraintComponent* constraint);

	UFUNCTION(BlueprintCallable)
	void BreakConnection(UBlockSMC* other);

	UFUNCTION(BlueprintCallable)
	void Detach(TArray<UBlockSMC*>& DetachedComps);

	UFUNCTION()
	void OnConstraintBroken(int32 ConstraintIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EBlockType GetBlockType();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsNetworkStatic();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsNetworkSemiStatic();

	UFUNCTION(BlueprintCallable)
	void DisableSimWhenSleep();


	void DisableSim();
	UFUNCTION()
	void DisableSim(UPrimitiveComponent* comp, FName bone);

	UFUNCTION(BlueprintCallable)
	void EnableSim();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FSocketData> GetFreeSockets(EBlockSocketType Type);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FSocketData> GetActorFreeSockets(EBlockSocketType Type) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UBlockSMC*> GetAttachedComponents();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UBlockSMC*> GetNetworkOfAttachedComponents(UBlockSMC* CompBefore);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsFree();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool AreSocketsAttached(FName socket, FSocketData& other);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AActor*> FindAttachedActors(AActor* actoreBefore);

	UFUNCTION(BlueprintCallable)
	void SetWireframe(const bool wireframe);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsWireframe() const;

	void ResetMaterialToDefault();

protected:

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, FSocketData> SocketMap;

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, UPhysicsConstraintComponent*> ConstraintMap;

	bool bIsWireframe = false;

private:
	UPROPERTY()
	UMaterialInterface* DefaultMaterial;

};
