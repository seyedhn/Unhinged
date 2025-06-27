#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Components/SceneComponent.h"
#include "WorldPartition/WorldPartitionStreamingSource.h"
#include "WormholeStreamingSourceComp.generated.h"


UCLASS(Meta = (BlueprintSpawnableComponent), HideCategories = (Tags, Sockets, ComponentTick, ComponentReplication, Activation, Cooking, Events, AssetUserData, Collision, Navigation))
class UWormholeStreamingSourceComp : public USceneComponent, public IWorldPartitionStreamingSourceProvider
{
	GENERATED_BODY()

public:

	UWormholeStreamingSourceComp(const FObjectInitializer& ObjectInitializer);

	virtual void OnRegister() override;
	virtual void OnUnregister() override;

	/** Enable the component */
	UFUNCTION(BlueprintCallable, Category = "Streaming")
	void EnableStreamingSource() { bStreamingSourceEnabled = true; }

	/** Disable the component */
	UFUNCTION(BlueprintCallable, Category = "Streaming")
	void DisableStreamingSource() { bStreamingSourceEnabled = false; }

	/** Returns true if the component is active. */
	UFUNCTION(BlueprintPure, Category = "Streaming")
	bool IsStreamingSourceEnabled() const { return bStreamingSourceEnabled; }

	// IWorldPartitionStreamingSourceProvider interface
	virtual bool GetStreamingSource(FWorldPartitionStreamingSource& OutStreamingSource) const override;
	virtual const UObject* GetStreamingSourceOwner() const override { return this; }

	/** Returns true if streaming is completed for this streaming source component. */
	UFUNCTION(BlueprintCallable, Category = "Streaming")
	bool IsStreamingCompleted() const;

	/** When TargetGrids or TargetHLODLayers are specified, this indicates the behavior. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Streaming")
	EStreamingSourceTargetBehavior TargetBehavior;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Streaming")
	EStreamingSourcePriority Priority;

private:
	/** Whether this component is enabled or not */
	UPROPERTY(EditAnywhere, Interp, Category = "Streaming")
	bool bStreamingSourceEnabled;

	UPROPERTY(EditAnywhere, Category = "Streaming")
	EStreamingSourceTargetState TargetState;
};