#pragma once

#include "CoreMinimal.h"
#include "SplineComp.h"
#include "../Block/BlockActor.h"
#include "SplineBlock.generated.h"




UCLASS(Blueprintable, BlueprintType)
class THEGAME_API ASplineBlock : public ABlockActor
{
	GENERATED_BODY()

public:

	ASplineBlock();
	virtual void BeginPlay() override;
	void OnActorSpawnLoaded_Implementation(bool RewindLoad) override;
	void OnActorReloaded_Implementation(bool RewindLoad) override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Initialise();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USplineComp* SplineComp;

protected:


private:


};
