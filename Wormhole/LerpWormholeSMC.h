
#pragma once

#include "WormholeSMCBase.h"
#include "Components/AudioComponent.h"
#include "LerpWormholeSMC.generated.h"

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API ULerpWormholeSMC : public UWormholeSMCBase
{
	GENERATED_BODY()

public:

	ULerpWormholeSMC();
	virtual void BeginPlay() override;
	virtual void TeleportCharacter(AActor* Character) override;

	void EnableCharacterMovement(ACharacter* Character, bool Enable);
	void TeleportEffect(bool Enable);
	void SetDistanceBasedPlayRate();

	UFUNCTION()
	void OnTeleportUpdated(float alpha);
	UFUNCTION()
	void OnTeleportFinished();

	UPROPERTY()
	UTimelineComponent* TeleportTimeline;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCurveFloat* TeleportCurve;

	UPROPERTY()
	UAudioComponent* AudioComp;

protected:


private:



};
