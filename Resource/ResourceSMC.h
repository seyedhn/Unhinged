
#pragma once

#include "CoreMinimal.h"
#include "../Save/SaveGameInterface.h"
#include "../Fuse/FuseInterface.h"
#include "../Rewind/RewindInterface.h"
#include "ResourceSMC.generated.h"



UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API UResourceSMC : public UStaticMeshComponent, public ISaveGameInterface, public IFuseInterface, public IRewindInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnItself(const FName Key, const int32 Amount, const FVector Location, const FVector Impulse);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DestroySelf();
	virtual void DestroySelf_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FullyDestroy();
	virtual void FullyDestroy_Implementation();

	UFUNCTION(BlueprintCallable)
	void SetCreationMethod(EComponentCreationMethod Method);


	//Rewind Interface//
	bool IsCharged_Implementation(URewindComp*& RewindComp) const override;
	void BecomeCharged_Implementation(bool Charged, UPrimitiveComponent* HitComp) override;
	bool IsRewindable_Implementation() const override;


protected:



};
