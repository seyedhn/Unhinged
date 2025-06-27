// Copyright Epic Games, Inc. All Rights Reserved.

#include "WormholeStreamingSourceComp.h"
#include "WorldPartition/WorldPartitionSubsystem.h"
#include "SceneManagement.h"
#include "Engine/World.h"


UWormholeStreamingSourceComp::UWormholeStreamingSourceComp(const FObjectInitializer& ObjectInitializer)
	: 
	 TargetBehavior(EStreamingSourceTargetBehavior::Include)
	, Priority(EStreamingSourcePriority::Normal)
	, bStreamingSourceEnabled(false)
	, TargetState(EStreamingSourceTargetState::Activated)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWormholeStreamingSourceComp::OnRegister()
{
	Super::OnRegister();

	UWorld* World = GetWorld();

	UWorldPartitionSubsystem* WorldPartitionSubsystem = GetWorld()->GetSubsystem<UWorldPartitionSubsystem>();
	check(WorldPartitionSubsystem);
	WorldPartitionSubsystem->RegisterStreamingSourceProvider(this);
}

void UWormholeStreamingSourceComp::OnUnregister()
{
	Super::OnUnregister();

	UWorld* World = GetWorld();

	UWorldPartitionSubsystem* WorldPartitionSubsystem = GetWorld()->GetSubsystem<UWorldPartitionSubsystem>();
	check(WorldPartitionSubsystem);
	verify(WorldPartitionSubsystem->UnregisterStreamingSourceProvider(this));
}


bool UWormholeStreamingSourceComp::GetStreamingSource(FWorldPartitionStreamingSource& OutStreamingSource) const
{
	if (bStreamingSourceEnabled)
	{
		USceneComponent* Parent = GetAttachParent();

		OutStreamingSource.Name = Parent->GetFName();
		OutStreamingSource.Location = Parent->GetComponentLocation();
		OutStreamingSource.Rotation = Parent->GetComponentRotation();
		OutStreamingSource.TargetState = TargetState;
		OutStreamingSource.TargetBehavior = TargetBehavior;
		OutStreamingSource.Priority = Priority;

		return true;
	}
	return false;
}

bool UWormholeStreamingSourceComp::IsStreamingCompleted() const
{
	if (UWorldPartitionSubsystem* WorldPartitionSubsystem = UWorld::GetSubsystem<UWorldPartitionSubsystem>(GetWorld()))
	{
		return WorldPartitionSubsystem->IsStreamingCompleted(this);
	}
	return true;
}



