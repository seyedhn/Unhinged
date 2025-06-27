
#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Components/SphereComponent.h"
#include "WormholeSMCBase.generated.h"


UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API UWormholeSMCBase : public USphereComponent
{
	GENERATED_BODY()

public:

	UWormholeSMCBase();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ActivateWormholeSMC(bool Activate);

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void SetOtherWormhole();
	void InitialiseTimeline(UTimelineComponent* Timeline, UCurveFloat* Curve, FName UpdateFuncName, FName FinishedFuncName, float TimelineLength, float PlayRate);
	void ActivateCompCollisionResponseOnTimer(UPrimitiveComponent* comp);


	//CHARACTER INTSANT TELEPORT
	UFUNCTION()
	virtual void TeleportCharacter(AActor* Character) {};

	//COMPONENT TELEPORT
	UFUNCTION()
	void TeleportComponent(UPrimitiveComponent* Comp);
	UFUNCTION()
	void OnCompSuctionUpdated(float alpha);
	UFUNCTION()
	void OnCompSuctionFinished();


	UPROPERTY()
	UTimelineComponent* CompSuctionTimeline;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCurveFloat* SuctionCurve;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<USoundBase> TeleportSoundSoftRef;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<USoundBase> SuctionSoundSoftRef;


protected:

	UWormholeSMCBase* OtherWormhole;
	AActor* SuckedActor;
	UPrimitiveComponent* SuckedComp;
	FVector StartLocation;
	FVector TargetLocation;
	FVector ExitLocation;

private:




};
