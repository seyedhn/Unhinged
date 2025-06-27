#include "EquipmentHandler.h"



UEquipmentHandler::UEquipmentHandler()
{
    Quickslots = { FName(), FName(), FName() };
}


void UEquipmentHandler::AddEquipmentToFreeSlot(FName Key)
{
    for (FName& slot : Quickslots)
    {
        if (slot.IsNone())
        {
            slot = Key;
            return;
        }
    }
}

void UEquipmentHandler::RemoveEquipmentFromSlot(FName Key)
{
    int32 Index;
    if (IsItemInQuickslot(Key, Index))
    {
        Quickslots[Index] = NAME_None;
    }
}

void UEquipmentHandler::SetQuickslot(FName Key, int32 Index)
{
    int32 OldIndex;
    if(IsItemInQuickslot(Key, OldIndex))
    {
       Quickslots[OldIndex] = Quickslots[Index];
    }
    Quickslots[Index] = Key;
}

bool UEquipmentHandler::IsItemInQuickslot(FName Key, int32& Index)
{
    return Quickslots.Find(Key, Index);
}

void UEquipmentHandler::UnequipWeapon(bool EquipLastWeapon)
{
  if (EquipLastWeapon)
  {
    PrepareSwapEquipment(LastHeldEquipment);
    return;
  }
  PrepareSwapEquipment(NAME_None);
}

void UEquipmentHandler::EquipGizmoToTab(uint8 TabIndex)
{
  EquipGizmoToTabWithPayload(TabIndex, FInstancedStruct());
}

void UEquipmentHandler::EquipGizmoToTabWithPayload(uint8 TabIndex, const FInstancedStruct& _Payload)
{
  EquipWithPayload(TEXT("Device"), TabIndex, _Payload);
}

void UEquipmentHandler::EquipWatchToTabWithPayload(const uint8 TabIndex, const FInstancedStruct& _Payload)
{
  EquipWithPayload(TEXT("Watch"), TabIndex, _Payload);
}

void UEquipmentHandler::EquipForgeWithPayload(const uint8 TabIndex, const FInstancedStruct& _Payload)
{
  EquipWithPayload(TEXT("Forge"), TabIndex, _Payload);
}

void UEquipmentHandler::EquipWithPayload(const FName EquipmentName, const uint8 TabIndex, const FInstancedStruct& _Payload)
{
  EquipPayload = TabIndex;
  Payload = _Payload;
  PrepareSwapEquipment(EquipmentName);
}
