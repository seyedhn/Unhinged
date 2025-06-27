#include "SkyManager.h"
#include "Kismet/GameplayStatics.h"
#include "../Tools/OurSystemLibrary.h"
#include "../Manager/ManagerSettings.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "../Messaging/PayloadStructs.h"
#include "../Messaging/ChannelTags.h"

void ASkyManager::LoadManager(bool bRewindLoad)
{
  SetUDS();
  ApplySkyConfigByName(CurrentSkyName);
  ChangeTimeOfDay(CurrentTime);
}

void ASkyManager::SaveManager()
{
  SetSkyConfigName();
  SaveTimeOfDay();
}

void ASkyManager::SetSkyConfigName()
{
  CurrentSkyName = FName(CurrentSky.LoadSynchronous()->Name);
}

bool ASkyManager::GetSkyConfigs(TSubclassOf<UPrimaryDataAsset> Class, FName ConfigName, UPrimaryDataAsset*& OutConfig) const
{
  TArray<FAssetData> AssetData = UOurSystemLibrary::FindDataAssetsOfClass(Class.Get(), true, true);
  FName TagName = TEXT("Name");
  FName temp_tag;

  for (FAssetData asset : AssetData)
  {
    
    asset.GetTagValue(TagName, temp_tag);
    if (asset.AssetName == ConfigName)
    {
      OutConfig = Cast<UPrimaryDataAsset>(asset.GetAsset());
      return true;
    }
  }

  return false;
}
