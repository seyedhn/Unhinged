// Copyright 2024 Seyed Nasrollahi. All Rights Reserved.

#include "ItemsDataLibrary.h"
#include "ItemTags.h"
#include "../Manager/ManagerSubsystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "ItemDataSubsystem.h"

bool UItemsDataLibrary::GetItemDrops(const UObject* WorldContextObject, const FName ItemKey, TArray<FName>& Drops)
{
  UItemDataSubsystem& ItemDataSubsystem = UItemDataSubsystem::Get(WorldContextObject);
  
  if (!ItemDataSubsystem.ItemHasProperty<FItemDropData>(ItemKey)) return false;

  FItemDropData DropData = ItemDataSubsystem.GetItemProperty<FItemDropData>(ItemKey);

  for (auto& drop : DropData.Drops)
  {
    for (int i = 0; i < drop.Value; i++)
    {
      Drops.Add(drop.Key);
    }
  }

  for (auto& randomDrop : DropData.RandomDrops)
  {
    if (UKismetMathLibrary::RandomBoolWithWeight(randomDrop.Value))
    {
      Drops.Add(randomDrop.Key);
    }
  }

  return true;
}

EUpgradeType UItemsDataLibrary::GetItemUpgradeType(const UObject* WorldContextObject, const FName ItemKey)
{
  FGameplayTagContainer ItemTags = UItemDataSubsystem::Get(WorldContextObject).GetItemTags(ItemKey);

  if (ItemTags.HasTag(FItemTags::Get().Block) || ItemTags.HasTag(FItemTags::Get().Structure)) return EUpgradeType::Block;
  if (ItemKey.ToString().Contains("Inventory")) return EUpgradeType::Inventory;
  if (ItemKey.ToString().Contains("Tier")) return EUpgradeType::Tier;

  return EUpgradeType::EquipmentUpgrade;
}

EPurchaseType UItemsDataLibrary::GetItemPurchaseType(const UObject* WorldContextObject, const FItemPurchaseData PurchaseData)
{
  return PurchaseData.GetPurchaseType();
}
