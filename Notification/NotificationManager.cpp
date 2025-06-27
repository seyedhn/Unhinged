#include "NotificationManager.h"
#include "../Messaging/ChannelTags.h"
#include "../Tools/OurSystemLibrary.h"
#include "../Manager/ManagerSubsystem.h"

void ANotificationManager::BeginPlay()
{
  Super::BeginPlay();

  UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
  MessageSubsystem.RegisterListener(FChannelTags::Get().Objective, this, &ANotificationManager::CreateObjective);

}

UNotificationWidgetBase* ANotificationManager::FindWidget(const TArray<UNotificationWidgetBase*>& Stack, const FName Key) const
{
  for (int32 i=0; i < Stack.Num(); i++)
  {
    if (Stack[i]->Key == Key)
    {
      return Stack[i];
    }
  }
  return nullptr;
}



//-------------------OBJECTIVE-----------------------------//

void ANotificationManager::CreateObjective(FGameplayTag Channel, const FPayloadKey& Payload)
{
  FName Key = Payload.Key;

  if (!AccomplishedObjectives.Contains(Key) && !ObjectiveNamesStack.Contains(Key) && HintsPDA->Objectives.Contains(Key))
  {
     ShowObjective(Key);
     ObjectiveNamesStack.Add(Key);
     UOurSystemLibrary::ExecuteFunctionVoid(this, Key);
  }
}

void ANotificationManager::RemoveObjective(FName Key)
{
  if (UNotificationWidgetBase* Widget = FindWidget(ObjectiveStack, Key))
  {
    Widget->RemoveWidget();
  }
  MessageHandles.FindRef(Key).Unregister();
  MessageHandles.Remove(Key);
  ObjectiveNamesStack.Remove(Key);
  AccomplishedObjectives.Add(Key);
}



//OBJECTIVES//

void ANotificationManager::EatFood()
{
  FGameplayMessageListenerHandle Handle = UGameplayMessageSubsystem::Get(this).RegisterListener(FChannelTags::Get().ItemConsumed, this, &ANotificationManager::AccomplishEatFood);
  MessageHandles.Add(TEXT("EatFood"), Handle);
}

void ANotificationManager::AccomplishEatFood(FGameplayTag Channel, const FPayloadKey& Payload)
{
  FName Key = TEXT("EatFood");
  RemoveObjective(Key);
}

void ANotificationManager::MakeSave()
{
  FGameplayMessageListenerHandle Handle = UGameplayMessageSubsystem::Get(this).RegisterListener(FChannelTags::Get().GameSaved, this, &ANotificationManager::AccomplishMakeSave);
  MessageHandles.Add(TEXT("MakeSave"), Handle);
}

void ANotificationManager::AccomplishMakeSave(FGameplayTag Channel, const FPayloadEmpty& Payload)
{
  FName Key = TEXT("MakeSave");
  RemoveObjective(Key);
 // CreateObjective(FChannelTags::Get().Objective, FPayloadKey(TEXT("FindShelter")));
  CreateObjective(FChannelTags::Get().Objective, FPayloadKey(TEXT("StartTrial")));

}

void ANotificationManager::StartTrial()
{
  FGameplayMessageListenerHandle Handle = UGameplayMessageSubsystem::Get(this).RegisterListener(FChannelTags::Get().Trial, this, &ANotificationManager::AccomplishStartTrial);
  MessageHandles.Add(TEXT("StartTrial"), Handle);
}

void ANotificationManager::AccomplishStartTrial(FGameplayTag Channel, const FPayloadBool& Payload)
{
  FName Key = TEXT("StartTrial");
  RemoveObjective(Key);
  CreateObjective(FChannelTags::Get().Objective, FPayloadKey(TEXT("EndTrial")));
}

void ANotificationManager::EndTrial()
{
  FGameplayMessageListenerHandle Handle = UGameplayMessageSubsystem::Get(this).RegisterListener(FChannelTags::Get().Trial, this, &ANotificationManager::AccomplishEndTrial);
  MessageHandles.Add(TEXT("EndTrial"), Handle);
}

void ANotificationManager::AccomplishEndTrial(FGameplayTag Channel, const FPayloadKeyInt& Payload)
{
  FName Key = TEXT("EndTrial");
  RemoveObjective(Key);
  CreateObjective(FChannelTags::Get().Objective, FPayloadKey(TEXT("TierUp")));
  CreateObjective(FChannelTags::Get().Objective, FPayloadKey(TEXT("BuyUpgrade")));
  CreateObjective(FChannelTags::Get().Objective, FPayloadKey(TEXT("Craft")));
  CreateObjective(FChannelTags::Get().Objective, FPayloadKey(TEXT("Sleep")));

  TArray<FName> blockkeys = { TEXT("FirePit"), TEXT("Transmitter"), TEXT("HyperMail"), TEXT("Bed") };
  for (FName key : blockkeys)
  {
    UManagerSubsystem::Get(this).BlockManager->UnlockBlock(key);
  }
}

void ANotificationManager::FindShelter()
{
  FGameplayMessageListenerHandle Handle = UGameplayMessageSubsystem::Get(this).RegisterListener(FChannelTags::Get().Shelter, this, &ANotificationManager::AccomplishFindShelter);
  MessageHandles.Add(TEXT("FindShelter"), Handle);
}

void ANotificationManager::AccomplishFindShelter(FGameplayTag Channel, const FPayloadBool& Payload)
{
  FName Key = TEXT("FindShelter");
  RemoveObjective(Key);
}

void ANotificationManager::BuyUpgrade()
{
  FGameplayMessageListenerHandle Handle = UGameplayMessageSubsystem::Get(this).RegisterListener(FChannelTags::Get().HyperMail, this, &ANotificationManager::AccomplishBuyUpgrade);
  MessageHandles.Add(TEXT("BuyUpgrade"), Handle);
}

void ANotificationManager::AccomplishBuyUpgrade(FGameplayTag Channel, const FPayloadHyperMail& Payload)
{
  if (Payload.Upgrades.Num() > 0)
  {
    RemoveObjective(TEXT("BuyUpgrade"));
  }
}

void ANotificationManager::TierUp()
{
  FGameplayMessageListenerHandle Handle = UGameplayMessageSubsystem::Get(this).RegisterListener(FChannelTags::Get().TierUp, this, &ANotificationManager::AccomplishTierUp);
  MessageHandles.Add(TEXT("TierUp"), Handle);
}

void ANotificationManager::AccomplishTierUp(FGameplayTag Channel, const FPayloadInt& Payload)
{
  if(Payload.Amount == 2)
  RemoveObjective(TEXT("TierUp"));
}

void ANotificationManager::Craft()
{
  FGameplayMessageListenerHandle Handle = UGameplayMessageSubsystem::Get(this).RegisterListener(FChannelTags::Get().Craft, this, &ANotificationManager::AccomplishCraft);
  MessageHandles.Add(TEXT("Craft"), Handle);
}

void ANotificationManager::AccomplishCraft(FGameplayTag Channel, const FPayloadKeyIntMap& Payload)
{
  if (Payload.Deliveries.Num() > 0)
  {
    RemoveObjective(TEXT("Craft"));
  }

}

void ANotificationManager::Sleep()
{
  FGameplayMessageListenerHandle Handle = UGameplayMessageSubsystem::Get(this).RegisterListener(FChannelTags::Get().Sleep, this, &ANotificationManager::AccomplishSleep);
  MessageHandles.Add(TEXT("Sleep"), Handle);
}

void ANotificationManager::AccomplishSleep(FGameplayTag Channel, const FInstancedStruct& Payload)
{
  int32 Hours = Payload.Get<FPayloadInt>().Amount;
  if (Hours > 0)
  {
    RemoveObjective(TEXT("Sleep"));
  }
}

