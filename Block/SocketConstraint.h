// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

#include "SocketConstraint.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API USocketConstraint : public UPhysicsConstraintComponent
{
	GENERATED_BODY()


public:
	virtual void InitializeComponent() override;

private:


};
