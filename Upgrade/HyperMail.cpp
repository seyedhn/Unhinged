#include "HyperMail.h"
#include "../Resource/ResourceLibrary.h"
#include "../ItemsData/ItemPropertyStructs.h"
#include "../ItemsData/ItemTags.h"
#include "ItemDataSubsystem.h"
#include "Containers/Map.h"


void AHyperMail::BindToDevice()
{
  UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);

  Handle = MessageSubsystem.RegisterListener(FChannelTags::Get().HyperMail, this, &ThisClass::OnSubmitOrCancelOrder);
}

void AHyperMail::OnSubmitOrCancelOrder(FGameplayTag Channel, const FPayloadHyperMail& Payload)
{
  Consumables = Payload.Consumables;
  Upgrades = Payload.Upgrades;
  UnbindFromDevice();
  if (IsThereDelivery())
  {
    OnOrderPlaced();
  }

}

void AHyperMail::TryCollectDelivery()
{
  for (FName key : Upgrades)
  {
    FGameplayTag ChannelTag = UItemDataSubsystem::Get(this).ItemHasTag(key, FItemTags::Get().Equipment) ? FChannelTags::Get().EquipmentPickup : FChannelTags::Get().UpgradePickup;
    UGameplayMessageSubsystem::Get(this).BroadcastMessage(ChannelTag, FPayloadKey(key));
  }
  Upgrades.Empty();

  bool CollectionSuccessful = true;
  TMap<FName, int32> tempConsumables = Consumables;
  for (auto& elem : tempConsumables)
  {
    if (UResourceLibrary::TryCollectItem(this, elem.Key, elem.Value)) Consumables.Remove(elem.Key);
    else CollectionSuccessful = false;
  }
  if(CollectionSuccessful) DeliveryCollected();
}


void AHyperMail::UnbindFromDevice()
{
  UGameplayMessageSubsystem::Get(this).UnregisterListener(Handle);
}

bool AHyperMail::IsThereDelivery() const
{
  return Consumables.Num() > 0 || Upgrades.Num() > 0;
}


void AHyperMail::Serialize(FArchive& Ar)
{

 /*
  if (Ar.IsSaving())
  {
    TArray<TPair<FName, int32>> arr = Delivery.Array();
    if (arr.Num() > 0)
    {
      Ar << arr;
    }
    

    
    TArray<FName> keys;
    TArray<int32> values;
    Delivery.GenerateKeyArray(keys);
    Delivery.GenerateValueArray(values);
    Ar << keys;
    Ar << values;
    
  }
  else if (Ar.IsLoading())
  {
    TArray<TPair<FName, int32>> arr;
    Ar << arr;

    /
    Delivery.Empty(); // Clear the existing data

    int32 size = 0;
    TArray<FName> keys;
    TArray<int32> values;
    Ar << size;
   // Ar << keys;
   // Ar << values;
    if (size > 0)
    {
      for (int i = 0; i < keys.Num(); i++)
      {
        Delivery.Add(keys[i], values[i]);
      }
    }
    
  }*/
 // Ar << Delivery;
  Super::Serialize(Ar);

}
