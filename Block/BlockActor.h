// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataInterface.h"
#include "ItemDataSubsystem.h"
#include "../Actors/GameActor.h"
#include "../Save/SaveGameInterface.h"
#include "../Rewind/RewindInterface.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "BlockActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMaterialise);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDetachment);

class URewindComp;

UCLASS(Blueprintable, BlueprintType)
class THEGAME_API ABlockActor : public AGameActor, public IItemDataInterface, public IRewindInterface
{
	GENERATED_BODY()

public:
//	virtual void OnConstruction(const FTransform& Transform) override;
	ABlockActor();
	virtual void BeginPlay() override;
	virtual void PreInitializeComponents() override;
	void OnActorReloaded_Implementation(bool RewindLoad) override;
	void OnActorSpawnLoaded_Implementation(bool RewindLoad) override;
	FName GetItemKey_Implementation() const override; 
	bool IsItemActor_Implementation() const override;

	//Rewind Interface//
	bool IsCharged_Implementation(URewindComp*& RewindComp) const override;
	void BecomeCharged_Implementation(bool Charged, UPrimitiveComponent* HitComp) override;
	bool IsRewindable_Implementation() const override;


	void SetBlockData();
	void SetBlockType();
	void SetMass(UItemDataAsset* DA);
	void SetChargedMaterial();
	void TryDealDamage(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void TemporarilyDisableHitEvent(UPrimitiveComponent* HitComponent);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsNetworkStatic(ABlockActor* actoreBefore);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EBlockType GetBlockType() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<ABlockActor*> GetNetworkOfAttachedBlocks(ABlockActor* actoreBefore, EBlockSocketType Type);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	void GetAttachedBlocks(TSet<ABlockActor*> IgnoreBlocks, EBlockSocketType Type, TArray<ABlockActor*>& AttachedBlocks, TArray<FSocketData>& AttachedSockets, TArray<UBlockSMC*>& HostComps);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FSocketData> GetFreeSockets(EBlockSocketType Type) const;

	UFUNCTION(BlueprintCallable)
	void FlowObjectToNextBlock(ABlockActor* PreviousBlock, FName Object, EBlockSocketType SocketType);

	UFUNCTION(BlueprintCallable)
	void DestroyComponents(TArray<UActorComponent*> components);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (DeterminesOutputType = "CompClass"))
	UActorComponent* MakeComponent(TSubclassOf<UActorComponent> CompClass);

	UFUNCTION(BlueprintCallable)
	void EnableSim(bool Enable);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Materialize();
	virtual void Materialize_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	URewindComp* GetRewindComp() const;

	UFUNCTION(BlueprintCallable)
	void SetWireframe(const bool wireframe);

	UFUNCTION(BlueprintCallable)
	void DetachBlock();

	UFUNCTION(BlueprintCallable)
	void DestroyBlock();

	UFUNCTION(BlueprintCallable)
	void BindToEquipment(FGameplayTag Channel);
	UFUNCTION(BlueprintNativeEvent)
	void OnMessageReceivedFromEquipment(FGameplayTag Channel, const FInstancedStruct& Payload);
	virtual void OnMessageReceivedFromEquipment_Implementation(FGameplayTag Channel, const FInstancedStruct& Payload);


	void SetBlockTransform(FTransform transform);
	FTransform GetBlockTransform();
	float GetBlockVelSq();

	UPROPERTY(BlueprintReadOnly)
	TArray<UBlockSMC*> Comps;

	UPROPERTY(BlueprintReadOnly)
	UBlockSMC* RootComp;

	UPROPERTY(BlueprintAssignable)
	FMaterialise OnMaterialise;

	UPROPERTY(BlueprintAssignable)
	FDetachment OnDetached;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	bool bIsMaterialised = true;

	bool bIsGrabbed;


protected:
	FName Key;
	float Mass = 1;
	EBlockType BlockType;


	TArray<FTransform> CompLocalTransforms;

	UPROPERTY()
	FGameplayMessageListenerHandle MessageListenerHandle;
};
