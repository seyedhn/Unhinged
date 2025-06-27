// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PhysicsEngine/PhysicsConstraintActor.h"

#include "GrabberComponent.generated.h"


/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class THEGAME_API UGrabberComponent : public UPhysicsHandleComponent
{
	GENERATED_BODY()


public:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:


};
