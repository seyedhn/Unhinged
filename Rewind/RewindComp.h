// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Messaging/ChannelTags.h"
#include "Containers/CircularQueue.h"
#include "Containers/RingBuffer.h"
#include "Components/TimelineComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "../Block/BlockActor.h"
#include "RewindComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBlockRewindEndDelegate, URewindComp*, RewindComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBlockRewindProgressDelegate, float, alpha);

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API URewindComp : public UActorComponent
{
	GENERATED_BODY()

public:
	URewindComp();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void Initialise_Internal();

	UFUNCTION(BlueprintCallable)
	void InitialiseRewindComp(UObject* Owner, UPrimitiveComponent* RewindComp);

	//REWIND//

	void RecordTimestamps();
	UFUNCTION(BlueprintCallable)
	void BeginRewind();
	UFUNCTION()
	void LerpTransforms(float alpha);
	UFUNCTION()
	void TraverseTimestamp();
	UFUNCTION()
	void EndRewind();
	UFUNCTION(BlueprintCallable)
	void PauseRewind();
	UFUNCTION(BlueprintCallable)
	void ResumeRewind();
	UFUNCTION(BlueprintCallable)
	void FastRewind();
	UFUNCTION(BlueprintCallable)
	void ShiftRewindSpeed(int32 Direction);
	UFUNCTION(BlueprintCallable)
	void CancelRewind();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsRewinding() const;
	UFUNCTION()
	int32 GetTimestampLength() const;

	//SPLINE//
	UFUNCTION()
	void MakeSpline();
	UFUNCTION()
	void DestroySpline();

	//TIMELINE//
	void ConstructTimeline();
	void ConstructSpline();
	void InitialiseTimeline();
	UFUNCTION()
	void OnTimelineUpdated(float alpha);
	UFUNCTION()
	void OnTimelineFinished();

	UPROPERTY()
	UTimelineComponent* RewindTimeline;

	UPROPERTY(BlueprintAssignable)
	FBlockRewindEndDelegate OnRewindEnd;

	UPROPERTY(BlueprintAssignable)
	FBlockRewindProgressDelegate OnRewindProgressed;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta=(ExposeOnSpawn="true"))
	UObject* RewindOwner;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (ExposeOnSpawn = "true"))
	UPrimitiveComponent* RewindableComp;


protected:


	UPROPERTY(BlueprintReadOnly)
	USplineComponent* Spline;

	UPROPERTY(BlueprintReadWrite)
	TArray<USplineMeshComponent*> SplineComps;


private:

	static const uint16 StampLength = 512;
	uint16 RewindLength;
	bool bIsRewinding;
	uint16 Pointer;
	float TimerRate = 0.1;

	FTimerHandle StampTimer;
	FTimerHandle RewindTimer;

	TRingBuffer<FTransform> Timestamps;

	bool IsSimulatingPhysics;
};
