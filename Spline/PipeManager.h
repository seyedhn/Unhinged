#pragma once

#include "CoreMinimal.h"
#include "../Manager/ManagerBase.h"
#include "../Block/BlockActor.h"
#include "../Messaging/PayloadStructs.h"
#include "PipeNetwork.h"
#include "PipeManager.generated.h"





UCLASS(Blueprintable, BlueprintType)
class THEGAME_API APipeManager : public AManagerBase
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;
	void SaveManager() override;
	void LoadManager(bool RewindLoad) override;

protected:

	void TryEstablishNetwork(FGameplayTag Channel, const FPayloadKeyBlock& Payload);

	void EstablishNetwork(const FPipeNetwork& Network);

	UFUNCTION(BlueprintCallable)
	void AddBlockToNetwork(ABlockActor* block, UPARAM(ref) FPipeNetwork& Network);

	void RunFluidInNetwork(FGameplayTag Channel, const FPayloadKeyBlock& Payload);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = Variables)
	TSet<FPipeNetwork> PipeNetworks;

private:


};
