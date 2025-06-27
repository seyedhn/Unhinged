#pragma once

#include "CoreMinimal.h"
#include "BlockSMC.h"
#include "../Manager/ManagerBase.h"
#include "../Save/SaveGameInterface.h"
#include "../Messaging/ChannelTags.h"
#include "BlockManager.generated.h"


UCLASS(Blueprintable, BlueprintType)
class THEGAME_API ABlockManager : public AManagerBase
{
	GENERATED_BODY()

public:

	ABlockManager();
	virtual void LoadManager(bool bRewindLoad) override;

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetUnlockedBlocks(const FGameplayTagContainer& TagContainer) const;

	UFUNCTION(BlueprintCallable)
	void UnlockBlock(const FName block);

	UFUNCTION(BlueprintCallable)
	void AddConnectionSaveData(FSocketData socket1, FSocketData socket2);

	UFUNCTION(BlueprintCallable)
	void RemoveConnectionSaveData(FSocketData socket1, FSocketData socket2);

	void ConnectSockets(FSocketData socket1, FSocketData socket2);

private:

	UPROPERTY(SaveGame)
	TSet<FSocketPairData> ConnectedSockets;

	UPROPERTY(SaveGame)
	TSet<FName> UnlockedBlocks;



};
