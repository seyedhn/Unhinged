#include "FoliagePDA.h"
#include "ItemDataSubsystem.h"

TSoftObjectPtr<UNiagaraSystem> UFoliagePDA::GetVFX(const UObject* WorldContextObject, const FName Key) const
{
 FGameplayTag Tag = UItemDataSubsystem::Get(WorldContextObject).GetItemTags(Key).First();

  if (VFX.Contains(Tag))
  {
    return VFX.FindRef(Tag);
  }
  return nullptr;
}
