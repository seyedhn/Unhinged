#include "ResourceSMC.h"
#include "../Manager/ManagerSettings.h"
#include "ItemDataSubsystem.h"
#include "../Rewind/RewindComp.h"


void UResourceSMC::DestroySelf_Implementation()
{
 // Rename(*GetName().Append(FString::FromInt(FMath::Rand())));
  Rename();
   
}

void UResourceSMC::FullyDestroy_Implementation()
{
 // Rename(*GetName().Append("_PendingKill"));

  DestroyComponent();
}

void UResourceSMC::SetCreationMethod(EComponentCreationMethod Method)
{
  CreationMethod = Method;
 // Rename(TEXT("aaa"));
}

bool UResourceSMC::IsCharged_Implementation(URewindComp*& _RewindComp) const
{
  TArray<URewindComp*> RewindComps;
  GetOwner()->GetComponents<URewindComp>(RewindComps);
  for (auto& RewindComp : RewindComps)
  {
    if (RewindComp->RewindableComp == this)
    {
      _RewindComp = RewindComp;
      return true;
    }
  }
  return false;
   
}

void UResourceSMC::BecomeCharged_Implementation(bool Charged, UPrimitiveComponent* HitComp)
{
  if (Charged)
  {
    SetMaterial(0, GetDefault<UManagerSettings>()->BlockChargedMaterial.LoadSynchronous());
    URewindComp* RewindComp = Cast<URewindComp>(GetOwner()->AddComponentByClass(URewindComp::StaticClass(), false, FTransform(), true));
    RewindComp->RewindOwner = this;
    RewindComp->RewindableComp = this;
    GetOwner()->FinishAddComponent(RewindComp, false, FTransform());
    return;
  }
  
  SetMaterial(0, GetStaticMesh()->GetStaticMaterials()[0].MaterialInterface);


  URewindComp* RewindComp;
  if (IRewindInterface::Execute_IsCharged(this, RewindComp))
  {
    RewindComp->Rename();
    RewindComp->DestroyComponent();
  }


  /*
  TArray<URewindComp*> RewindComps;
  GetOwner()->GetComponents<URewindComp>(RewindComps);
  for(auto& RewindComp : RewindComps)
  {
    if (RewindComp->RewindableComp == this)
    {
      RewindComp->Rename();
      RewindComp->DestroyComponent();
      return;
    }
  }
  */
}

bool UResourceSMC::IsRewindable_Implementation() const
{
  return UItemDataSubsystem::Get(GetOwner()).ItemHasProperty<FItemPhysicalProperties>(this);
}