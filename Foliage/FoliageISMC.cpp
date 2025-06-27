#include "FoliageISMC.h"
#include "Kismet/KismetSystemLibrary.h"


void UFoliageISMC::OnActorReloaded_Implementation(bool RewindLoad)
{

  if (RewindLoad)
  {
    int32 lastindex = TempRemovedInstances.Num() - 1;
    for (int32 i = lastindex; i >= 0; --i)
    {
      AddInstance(TempRemovedInstances[i].Transform); 

      //This procedure is to make sure that the indeces are fixed up to the original.
      //Also it is essential to iterate the array backwards, making sure instances are added back in the order they were removed.
      FTransform transf;
      GetInstanceTransform(TempRemovedInstances[i].Index, transf);
      RemoveInstance(TempRemovedInstances[i].Index);
      AddInstance(transf);
    }
    TempRemovedInstances.Empty();
  }
  else
  {
    for (int i : RemovedInstances)
    {
      RemoveInstance(i);
    }
  }

  /* This is only the visuals. Need to take care of combustion manager too.
  TArray<int32> indeces;
  Burning.GenerateKeyArray(indeces);
  for (auto& index : indeces)
  {
    ExtinguishFire(index);
    SetCustomDataValue(index, 0, 0.f, true);
  }*/

}

void UFoliageISMC::OnActorSaved_Implementation()
{
  TempRemovedInstances.Empty();
}

TArray<int32> UFoliageISMC::GetRemovedInstances() const
{
  return RemovedInstances;
}

void UFoliageISMC::SetRemovedInstance(TArray<int32> instances)
{
  RemovedInstances = instances;
}

bool UFoliageISMC::IsBurntOrBurning(int32 Index) const
{
  return Burning.Contains(Index) || Burnt.Contains(Index);
}

void UFoliageISMC::ExtinguishFire(const int32 Index)
{
  if (Burning.Contains(Index))
  {
    FBurnFX BurnFX = Burning.FindRef(Index);
    BurnFX.Sound->Deactivate();
    BurnFX.Sound->DestroyComponent();
    BurnFX.VFX->Deactivate();
    Burning.Remove(Index);
  }
}
