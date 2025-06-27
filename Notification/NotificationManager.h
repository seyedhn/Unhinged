#pragma once

#include "CoreMinimal.h"
#include "../Manager/ManagerBase.h"
#include "../Messaging/PayloadStructs.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "HintsPDA.h"
#include "NotificationWidgetBase.h"
#include "NotificationManager.generated.h"


UCLASS(Blueprintable, BlueprintType)
class THEGAME_API ANotificationManager : public AManagerBase
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AddWidgetToStack(const TArray<UNotificationWidgetBase*>& Stack, UNotificationWidgetBase* NewWidget);
	UFUNCTION(BlueprintCallable)
	UNotificationWidgetBase* FindWidget(const TArray<UNotificationWidgetBase*>& Stack, const FName Key) const;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void RestackWidgets(const TArray<UNotificationWidgetBase*>& Stack, const int32 GapIndex);

	//OBJECTIVE
	void CreateObjective(FGameplayTag Channel, const FPayloadKey& Payload);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowObjective(const FName Key);
	UFUNCTION(BlueprintCallable)
	void RemoveObjective(const FName Key);








	//OBJECTIVES//
	UFUNCTION()
	void EatFood();
	void AccomplishEatFood(FGameplayTag Channel, const FPayloadKey& Payload);
	UFUNCTION()
	void MakeSave();
	void AccomplishMakeSave(FGameplayTag Channel, const FPayloadEmpty& Payload);
	UFUNCTION()
	void StartTrial();
	void AccomplishStartTrial(FGameplayTag Channel, const FPayloadBool& Payload);
	UFUNCTION()
	void EndTrial();
	void AccomplishEndTrial(FGameplayTag Channel, const FPayloadKeyInt& Payload);
	UFUNCTION()
	void FindShelter();
	void AccomplishFindShelter(FGameplayTag Channel, const FPayloadBool& Payload);
	UFUNCTION()
	void BuyUpgrade();
	void AccomplishBuyUpgrade(FGameplayTag Channel, const FPayloadHyperMail& Payload);
	UFUNCTION()
	void TierUp();
	void AccomplishTierUp(FGameplayTag Channel, const FPayloadInt& Payload);
	UFUNCTION()
	void Craft();
	void AccomplishCraft(FGameplayTag Channel, const FPayloadKeyIntMap& Payload);
	UFUNCTION()
	void Sleep();
	void AccomplishSleep(FGameplayTag Channel, const FInstancedStruct& Payload);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UHintsPDA* HintsPDA;

	UPROPERTY(BlueprintReadWrite)
	TArray<UNotificationWidgetBase*> NotificationStack;

	UPROPERTY(BlueprintReadWrite)
	TArray<UNotificationWidgetBase*> ObjectiveStack;

	UPROPERTY(BlueprintReadOnly)
	TSet<FName> ObjectiveNamesStack;

	UPROPERTY(BlueprintReadOnly, SaveGame)
	TSet<FName> AccomplishedObjectives;

	UPROPERTY(BlueprintReadOnly, SaveGame)
	TSet<FName> DisplayedHints;

	UPROPERTY(BlueprintReadOnly, SaveGame)
	TSet<FName> DisplayedTitleCards;

private:
	UPROPERTY()
	TMap<FName, FGameplayMessageListenerHandle> MessageHandles;

};
