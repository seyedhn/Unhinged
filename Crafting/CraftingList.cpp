#include "CraftingList.h"
#include "ItemDataSubsystem.h"
#include "../ItemsData/ItemsDataLibrary.h"
#include "../Messaging/ChannelTags.h"
#include "../ItemsData/ItemPropertyStructs.h"
#include "../Resource/ResourceLibrary.h"
#include "GameFramework/GameplayMessageSubsystem.h"


void UCraftingList::AddItemToMap(FName ItemKey, UWidget* Widget)
{
  CraftItems.Add(ItemKey, Widget);
}

TArray<FName> UCraftingList::GetListItems() const
{
  TArray<FName> keys;
  CraftItems.GenerateKeyArray(keys);
  return keys;
}

void UCraftingList::CancelCraft()
{
  UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().Craft, FPayloadKeyIntMap());
}

void UCraftingList::SubmitCraft()
{
  FPayloadKeyIntMap Items = GetCraftedItems();
  UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().Craft, Items);
}

FPayloadKeyIntMap UCraftingList::GetCraftedItems()
{
  TMap<FName, int32> Items;
  TArray<FName> Keys = GetListItems();

  for (auto& key : Keys)
  {    
    int32 Amount = 1;
    if (Items.Contains(key))
    {
      Amount += Items.FindRef(key);
    }
    Items.Add(key, Amount);
  }
  return FPayloadKeyIntMap(Items);
}

UWidget* UCraftingList::IsItemOnList(FName ItemKey, bool& Valid, int32& Amount)
{
  Valid = CraftItems.Contains(ItemKey);
  Amount = CraftItems.Num(ItemKey);
  return CraftItems.FindRef(ItemKey);
}

void UCraftingList::RemoveItemFromMap(FName ItemKey, UWidget* Widget)
{
  CraftItems.RemoveSingle(ItemKey, Widget);
}
