#include "StorageComp.h"
#include "ItemDataSubsystem.h"
#include "../ItemsData/ItemPropertyStructs.h"
#include "../Manager/ManagerSubsystem.h"
#include "../Manager/ManagerSettings.h"
#include "Kismet/GameplayStatics.h"


void UStorageComp::AddToStorage(const FName Item, const int32 Amount)
{
  UpdateStorage(Item, Amount);
}

void UStorageComp::DeductFromStorage(const FName Item, const int32 Amount)
{
  UpdateStorage(Item, -Amount);
}

void UStorageComp::UpdateStorage(const FName Item, const int32 Amount)
{
  int32 NewAmount = Amount + Storage.FindRef(Item);
  if (NewAmount <= 0)  Storage.Remove(Item);
  else Storage.Add(Item, NewAmount);

  UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().InventoryUpdated, FPayloadInventory(Item, NewAmount, this));
}

int32 UStorageComp::GetItemAmount(const FName Item) const
{
  return Storage.FindRef(Item);
}

int32 UStorageComp::GetItemStack(const FName Item) const
{
  return UItemDataSubsystem::Get(this).GetItemProperty<FItemInventoryData>(Item).StackAmount;
}

int32 UStorageComp::GetStorageCapacity() const
{
  return Capacity;
}

void UStorageComp::IncreaseStorageCapacity(int32 Amount)
{
  Capacity += Amount;
}

void UStorageComp::SetStorageCapacity(int32 Amount)
{
  Capacity = Amount;
}

int32 UStorageComp::GetNumOccupiedSlots() const
{
  int32 slots = 0;
  for (auto& item : Storage)
  {
    slots += FMath::CeilToInt((float)item.Value / (float)GetItemStack(item.Key));
  }
  return slots;
}

int32 UStorageComp::GetNumFreeSlots() const
{
  return Capacity - GetNumOccupiedSlots();
}

TArray<FName> UStorageComp::GetStackedSlots(TArray<int32>& Amounts) const
{
  TArray<FName> Keys;
  for (auto& item : Storage)
  {
    FName Key = item.Key;
    int32 Stack = GetItemStack(item.Key);
    int32 Amount = item.Value;
    int32 slots = Amount / Stack;
    int32 remainder = FMath::Modulo(Amount, Stack);

    for (int i = 0; i < slots; i++)
    {
      Keys.Add(Key);
      Amounts.Add(Stack);
    }
    if (remainder > 0)
    {
      Keys.Add(Key);
      Amounts.Add(remainder);
    }
  }
  return Keys;
}

int32 UStorageComp::GetNumItemsCraftable(const FName Key) const
{
  UItemDataSubsystem& ItemDataSubsystem = UItemDataSubsystem::Get(this);
  TArray<int32> SortArr;

  if (!ItemDataSubsystem.ItemHasProperty<FItemDropData>(Key))  return 99;
  TMap<FName, int32> ItemCost = ItemDataSubsystem.GetItemProperty<FItemDropData>(Key).Drops;
  if (ItemCost.Num() == 0) return 99;

  for (auto& ingredient : ItemCost)
  {
    SortArr.Add(GetItemAmount(ingredient.Key) / ingredient.Value);
  }
  SortArr.Sort();
  return SortArr[0];
}

TArray<FName> UStorageComp::GetItemsWithProperty(const UScriptStruct* PropertyType, TArray<int32>& Amounts) const
{
  UItemDataSubsystem& ItemDataSubsystem = UItemDataSubsystem::Get(this);
  TArray<FName> Keys;

  for (auto& item : Storage)
  {
    if (ItemDataSubsystem.ItemHasProperty(item.Key, PropertyType))
    {
      Keys.Add(item.Key);
      Amounts.Add(item.Value);
    }
  }
  return Keys;
}

TArray<FName> UStorageComp::GetItemsWithTag(const FGameplayTag& Tag, TArray<int32>& Amounts) const
{
    UItemDataSubsystem& ItemDataSubsystem = UItemDataSubsystem::Get(this);
    TArray<FName> Keys;

    for (auto& item : Storage)
    {
        if (ItemDataSubsystem.ItemHasTag(item.Key, Tag))
        {
            Keys.Add(item.Key);
            Amounts.Add(item.Value);
        }
    }
    return Keys;
}

bool UStorageComp::TryAddItem(FName Key, int32 Amount)
{
  int32 Stack = GetItemStack(Key);

  if (FMath::CeilToInt((float)Amount / (float)Stack) <= GetNumFreeSlots())
  {
    AddToStorage(Key, Amount);
    return true;
  }

  if (Storage.Contains(Key))
  {
    int32 ExistingFreeSpace = Stack - FMath::Modulo(Storage.FindRef(Key), Stack);
    if (FMath::CeilToInt((float)(Amount - ExistingFreeSpace) / (float)Stack) <= GetNumFreeSlots())
    {
      AddToStorage(Key, Amount);
      return true;
    }
  }
  return false;
}

bool UStorageComp::TryDeductItem(FName Key, int32 Amount)
{
    if (Storage.FindRef(Key) >= Amount)
    {
        DeductFromStorage(Key, Amount);
        return true;
    }
    return false;
}

void UStorageComp::DropStorage()
{
    FTransform transform = GetOwner()->GetTransform();
    AResourceBox* ResourceBox = GetWorld()->SpawnActor<AResourceBox>(GetDefault<UManagerSettings>()->ResourceBox.LoadSynchronous(), transform);
    ResourceBox->SetItems(Storage);
    Storage.Empty();
}
