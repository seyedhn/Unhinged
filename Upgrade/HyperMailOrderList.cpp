#include "HyperMailOrderList.h"
#include "ItemDataSubsystem.h"
#include "../ItemsData/ItemsDataLibrary.h"
#include "../Messaging/ChannelTags.h"
#include "../ItemsData/ItemPropertyStructs.h"
#include "../Resource/ResourceLibrary.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "../Manager/ManagerSubsystem.h"



void UHyperMailOrderList::AddItemToMap(FName ItemKey, UWidget* Widget)
{
  OrderItems.Add(ItemKey, Widget);
}

TArray<FName> UHyperMailOrderList::GetOrderListItems() const
{
  TArray<FName> keys;
  OrderItems.GenerateKeyArray(keys);
  return keys;
}

void UHyperMailOrderList::SubmitOrder()
{
  UManagerSubsystem::Get(this).UpgradeManager->UpdateHyperMailCredit(TotalCost);   //  UResourceLibrary::DeductFromInventory(this, TEXT("Currency"), -TotalCost, EInventoryType::Backpack);

  UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
  FPayloadHyperMail Delivery = GetOrderedUpgradesAndDeliveredItems();
  MessageSubsystem.BroadcastMessage(FChannelTags::Get().HyperMail, Delivery);
}

FPayloadHyperMail UHyperMailOrderList::GetOrderedUpgradesAndDeliveredItems()
{
  TMap<FName, int32> Consumables;
  TArray<FName> Upgrades;
  UItemDataSubsystem& ItemDataSubsystem = UItemDataSubsystem::Get(this);
  FItemPurchaseData PurchaseData;
  TArray<FName> Keys = GetOrderListItems();

  for (auto& key : Keys)
  {
    PurchaseData = ItemDataSubsystem.GetItemProperty<FItemPurchaseData>(key);
    int32 Amount = PurchaseData.DeliveredAmount;

    switch (PurchaseData.GetPurchaseType())
    {
    case EPurchaseType::Consumable:
      
      if (Consumables.Contains(key))
      {
        Amount += Consumables.FindRef(key);
      }
      Consumables.Add(key, Amount);
      break;

    case EPurchaseType::Upgrade:
      Upgrades.Add(key);
      break;

    case EPurchaseType::Sellable:
      UResourceLibrary::DeductFromBackpack(this, key, 1);
      break;
    }
  }
  //Deliveries are received and processed by HyperMail. Upgrades are received and processed by UpgradeManager
  return FPayloadHyperMail(Consumables, Upgrades);
}



void UHyperMailOrderList::CancelOrder()
{
  UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);

  MessageSubsystem.BroadcastMessage(FChannelTags::Get().HyperMail, FPayloadHyperMail());
}

UWidget* UHyperMailOrderList::IsItemOnList(FName ItemKey, bool& Valid, int32& Amount)
{
  Valid = OrderItems.Contains(ItemKey);
  Amount = OrderItems.Num(ItemKey);
  return OrderItems.FindRef(ItemKey);
}

void UHyperMailOrderList::RemoveItemFromMap(FName ItemKey, UWidget* Widget)
{
 // UWidget* tempWidget = *OrderItems.Find(ItemKey);
  OrderItems.RemoveSingle(ItemKey, Widget);
}
