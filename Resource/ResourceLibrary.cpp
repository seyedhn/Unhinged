// Copyright 2024 Seyed Nasrollahi. All Rights Reserved.

#include "ResourceLibrary.h"
#include "StorageComp.h"
#include "../Manager/ManagerSubsystem.h"
#include "../ItemsData/ItemTags.h"


void UResourceLibrary::SpawnItem(const UObject* WorldContextObject, const FName Key, const FVector Location, const FVector Impulse)
{
  UManagerSubsystem::Get(WorldContextObject).ResourceManager->ResourceContainer->SpawnItem(Key, 1, Location, Impulse);
}

void UResourceLibrary::SpawnItemWithAmount(const UObject* WorldContextObject, const FName Key, const int32 Amount, const FVector Location, const FVector Impulse)
{
  UManagerSubsystem::Get(WorldContextObject).ResourceManager->ResourceContainer->SpawnItem(Key, Amount, Location, Impulse);
}

int32 UResourceLibrary::GetNumItemsCraftable(const UObject* WorldContextObject, const FName Key)
{
  return UManagerSubsystem::Get(WorldContextObject).Avatar->Backpack->GetNumItemsCraftable(Key);
}

int32 UResourceLibrary::GetItemAmount(const UObject* WorldContextObject, const FName Key)
{
  return UManagerSubsystem::Get(WorldContextObject).Avatar->Backpack->GetItemAmount(Key);
}

void UResourceLibrary::DeductFromStorage(const UObject* WorldContextObject, UStorageComp* Storage, FName Key, int32 Amount)
{
  Storage->DeductFromStorage(Key, Amount);
}

void UResourceLibrary::DeductFromBackpack(const UObject* WorldContextObject, FName Key, int32 Amount)
{
  UManagerSubsystem::Get(WorldContextObject).Avatar->Backpack->DeductFromStorage(Key, Amount);
}

void UResourceLibrary::ConsumeFromBackpack(const UObject* WorldContextObject, FName Key)
{
    UGameplayMessageSubsystem::Get(WorldContextObject).BroadcastMessage(FChannelTags::Get().ItemConsumed, FPayloadKey(Key));
    DeductFromBackpack(WorldContextObject, Key, 1);
}

bool UResourceLibrary::TryDeductFromBackpack(const UObject* WorldContextObject, FName Key, int32 Amount)
{
    return UManagerSubsystem::Get(WorldContextObject).Avatar->Backpack->TryDeductItem(Key, Amount);
}

bool UResourceLibrary::TryAddToStorage(const UObject* WorldContextObject, UStorageComp* Storage, FName Key, int32 Amount)
{
  return Storage->TryAddItem(Key, Amount);
}

bool UResourceLibrary::TryCollectItem(const UObject* WorldContextObject, FName Key, int32 Amount)
{
  if (UManagerSubsystem::Get(WorldContextObject).Avatar->Backpack->TryAddItem(Key, Amount))
  {
    UGameplayMessageSubsystem::Get(WorldContextObject).BroadcastMessage(FChannelTags::Get().ItemCollected, FPayloadKeyInt(Key, Amount));
    return true;
  }
  UGameplayMessageSubsystem::Get(WorldContextObject).BroadcastMessage(FChannelTags::Get().Warning, FPayloadKey(TEXT("InventoryFull")));
  return false;
}

bool UResourceLibrary::TryCollectItems(const UObject* WorldContextObject, const TArray<FName>& Items, TArray<FName>& RemainingItems)
{
  bool AllCollected = true;
  for (FName item : Items)
  {
    if (!TryCollectItem(WorldContextObject, item, 1))
    {
      AllCollected = false;
      RemainingItems.Add(item);
    }
  }
  return AllCollected;
}

bool UResourceLibrary::TryCollectDelivery(const UObject* WorldContextObject, const FName Item, const int32 Amount)
{
    UItemDataSubsystem& ItemDataSubsystem = UItemDataSubsystem::Get(WorldContextObject);
    UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(WorldContextObject);
    FGameplayTag ItemTag = ItemDataSubsystem.GetItemTags(Item).First();
    FGameplayTag ChannelTag;
    if (ItemTag == FItemTags::Get().Upgrade) { MessageSubsystem.BroadcastMessage(FChannelTags::Get().UpgradePickup, FPayloadKey(Item)); return true; }

    if (ItemTag == FItemTags::Get().Equipment){ MessageSubsystem.BroadcastMessage(FChannelTags::Get().EquipmentPickup, FPayloadKey(Item)); return true;}

  return TryCollectItem(WorldContextObject, Item, Amount);
}

bool UResourceLibrary::TryTransferItem(const UObject* WorldContextObject, UStorageComp* From, UStorageComp* To, FName Key, int32 Amount)
{
  if (TryAddToStorage(WorldContextObject, To, Key, Amount))
  {
    DeductFromStorage(WorldContextObject, From, Key, Amount);
    return true;
  }
  return false;
}
