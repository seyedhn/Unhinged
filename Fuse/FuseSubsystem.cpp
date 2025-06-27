// Fill out your copyright notice in the Description page of Project Settings.

#include "FuseSubsystem.h"
#include "../Manager/ManagerSettings.h"
#include "Kismet/GameplayStatics.h"
#include "../Tools/OurSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"

void UFuseSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
  SetAllRecipesData();
}

void UFuseSubsystem::Deinitialize()
{
}


bool UFuseSubsystem::TryFuse(UPrimitiveComponent* Object1, UPrimitiveComponent* Object2, int32 Amount)
{
  if (Object1 && Object2 && Object1->Implements<UFuseInterface>() && Object2->Implements<UFuseInterface>())
  {
    FName FusedItem = ValidateFuse(Object1, Object2);
    if (!FusedItem.IsNone())
    {
      Fuse(Object1, Object2, FusedItem, Amount);
      return true;
    }
  }
  return false;
}



FName UFuseSubsystem::ValidateFuse(UPrimitiveComponent* Object1, UPrimitiveComponent* Object2)
{
  FFuseRecipe recipe = FuseDA->FindRecipe(IItemDataInterface::Execute_GetItemKey(Object1), IItemDataInterface::Execute_GetItemKey(Object2));
  FName FusedItem = recipe.FusedItem;
  if (!FusedItem.IsNone() && ValidateFuseImpact(Object1, Object2))
  {
      return FusedItem;
  }
  return NAME_None;
}

bool UFuseSubsystem::ValidateFuseImpact(UPrimitiveComponent* Object1, UPrimitiveComponent* Object2)
{
  return ((Object1->GetComponentVelocity() - Object2->GetComponentVelocity()).SquaredLength() > 50000);
}

void UFuseSubsystem::Fuse(UPrimitiveComponent* Object1, UPrimitiveComponent* Object2, FName FusedItem, int32 Amount)
{
  IFuseInterface::Execute_TransformFuseObject(Object2, FusedItem, Amount);
  IFuseInterface::Execute_DestroyFusedObject(Object1);
  UGameplayStatics::PlaySoundAtLocation(this, GetDefault<UManagerSettings>()->FuseSound.LoadSynchronous(), Object2->GetComponentLocation());
  UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, GetDefault<UManagerSettings>()->FuseFX.LoadSynchronous(), Object2->GetComponentLocation(), FRotator(0,0,1), FVector(1.f), false, true, ENCPoolMethod::None, false);
}




void UFuseSubsystem::SetAllRecipesData()
{
  TArray<FAssetData> AssetData = UOurSystemLibrary::FindDataAssetsOfClass(UFusePDA::StaticClass(), true, true);
  if (AssetData.Num() > 0)
  {
    FuseDA = Cast<UFusePDA>(AssetData[0].GetAsset());
  }
}