#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Containers/SpscQueue.h"
#include "DuctFlowComponent.generated.h"





USTRUCT(Blueprintable)
struct FDuctFlow
{
	GENERATED_BODY();

	FDuctFlow() {}

	FDuctFlow(FName Key, ABlockActor* _PreviousBlock)
		: Object(Key), PreviousBlock(_PreviousBlock)
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FName Object;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	ABlockActor* PreviousBlock = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FTimerHandle Timer;

};


UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API UDuctFlowComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void Initialise(USplineComponent* spline);

	UFUNCTION(BlueprintCallable)
	void ObjectFlows(FName object, ABlockActor* PreviousBlock);

	void PlayStartAndMiddleSounds(float TimerLength);

	void ExitObject();

protected:

	UPROPERTY()
	USplineComponent* Spline;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USoundBase* ProcessSound;

private:

	TSpscQueue<FDuctFlow> Queue;

};
