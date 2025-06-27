#include "CraftingTable.h"
#include "../Resource/ResourceLibrary.h"
#include "ItemDataSubsystem.h"
#include "Containers/Map.h"


void ACraftingTable::BindToGizmo()
{
  UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);

  Handle = MessageSubsystem.RegisterListener(FChannelTags::Get().Craft, this, &ThisClass::OnSubmitOrCancelCraft);
}

void ACraftingTable::OnSubmitOrCancelCraft(FGameplayTag Channel, const FPayloadKeyIntMap& Payload)
{
  Items = Payload.Deliveries;
  UnbindFromGizmo();
  if (IsThereDelivery())
  {
    OnCraft();
  }

}

void ACraftingTable::TryCollectDelivery()
{
  UE_LOG(LogScript, Warning, TEXT("Try Collect Delivery at Crafting Table"));

  bool CollectionSuccessful = true;
  TMap<FName, int32> tempItems = Items;
  for (auto& elem : tempItems)
  {
    if (UResourceLibrary::TryCollectItem(this, elem.Key, elem.Value)) Items.Remove(elem.Key);
    else CollectionSuccessful = false;
  }
  if(CollectionSuccessful) DeliveryCollected();
}


void ACraftingTable::UnbindFromGizmo()
{
  Handle.Unregister();
}

bool ACraftingTable::IsThereDelivery() const
{
  return Items.Num() > 0;
}

