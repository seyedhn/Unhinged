
#pragma once

#include "WormholeSMCBase.h"
#include "InstantWormholeSMC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterTeleportedDelegate);

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API UInstantWormholeSMC : public UWormholeSMCBase
{
	GENERATED_BODY()

public:

	UInstantWormholeSMC();
	virtual void BeginPlay() override;
	virtual void TeleportCharacter(AActor* Character) override;


	UFUNCTION()
	void OnSuctionUpdated(float alpha);
	UFUNCTION()
	void OnSuctionFinished();

	UPROPERTY(BlueprintAssignable)
	FCharacterTeleportedDelegate OnCharacterTeleported;

	UPROPERTY()
	UTimelineComponent* SuctionTimeline;


protected:


private:



};
