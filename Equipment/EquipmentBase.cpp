#include "EquipmentBase.h"
#include "ItemDataSubsystem.h"



void AEquipmentBase::BeginPlay()
{
  Super::BeginPlay();

  AnimLayer = Anim_DA->AnimLayer;
  Type = Anim_DA->Type;

  if (UItemDataSubsystem::Get(this).ItemHasProperty<FItemDamager>(Name))
  {
    DamagerProperties = UItemDataSubsystem::Get(this).GetItemProperty<FItemDamager>(Name);
  }
}

FName AEquipmentBase::GetName() const
{
  return Name;
}

EEquipmentState AEquipmentBase::GetEquipmentState() const
{
  return EquipmentState;
}

void AEquipmentBase::UpdateState(EEquipmentState State)
{
  EquipmentState = State;
}
