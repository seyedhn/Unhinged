#include "UpgradeManager.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "ItemDataSubsystem.h"
#include "../Manager/ManagerSubsystem.h"
#include "../ItemsData/ItemPropertyStructs.h"
#include "../Tools/OurSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"



AUpgradeManager::AUpgradeManager()
{
}

void AUpgradeManager::BeginPlay()
{
  Super::BeginPlay();

  TArray<FAssetData> AssetData = UOurSystemLibrary::FindDataAssetsOfClass(UTrialsPDA::StaticClass(), true, true);
  if (AssetData.Num() > 0)
  {
    TrialTiersDA = Cast<UTrialsPDA>(AssetData[0].GetAsset());
  }

  UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);

  MessageSubsystem.RegisterListener(FChannelTags::Get().UpgradePickup, this, &ThisClass::OnScriptPickup);
  MessageSubsystem.RegisterListener(FChannelTags::Get().HyperMail, this, &ThisClass::OnUpgradeOrdered);
  MessageSubsystem.RegisterListener(FChannelTags::Get().EquipmentPickup, this, &ThisClass::OnEquipmentPickup);
  MessageSubsystem.RegisterListener(FChannelTags::Get().Trial, this, &ThisClass::OnRewardClaimed);
}

//-------------------------SCRIPT----------------------------//

bool AUpgradeManager::IsScriptInstalled(const FName Key) const
{
    return InstalledScripts.Contains(Key);
}

bool AUpgradeManager::IsScriptUnlocked(const FName Key) const
{
  return UnlockedScripts.Contains(Key);
}

bool AUpgradeManager::IsUpgradeOrdered(const FName Key) const
{
  return OrderedUpgrades.Contains(Key);
}

bool AUpgradeManager::AreThereUninstalledScripts() const
{
  return (InstalledScripts.Num() - UnlockedScripts.Num()) != 0;
}

int32 AUpgradeManager::GetNumOrderedUpgrades() const
{
  return OrderedUpgrades.Num();
}

TSet<FName> AUpgradeManager::GetOrderedUpgrades() const
{
  return OrderedUpgrades;
}

void AUpgradeManager::OnScriptPickup(FGameplayTag Channel, const FPayloadKey& Payload)
{
  UnlockedScripts.Add(Payload.Key);
}

void AUpgradeManager::InstallScript(const FName ScriptKey)
{
  InstalledScripts.Add(ScriptKey);
  UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().UpgradeInstalled, FPayloadKey(ScriptKey));

  FItemPurchaseData ScriptData = UItemDataSubsystem::Get(this).GetItemProperty<FItemPurchaseData>(ScriptKey);
  if (!ScriptData.DeliveredItems.IsEmpty())
  {
    for (FName key : ScriptData.DeliveredItems)
    {
      UManagerSubsystem::Get(this).BlockManager->UnlockBlock(key);
    }
    return;
  }
  UOurSystemLibrary::ExecuteFunctionVoid(this, ScriptKey);
}

void AUpgradeManager::OnUpgradeOrdered(FGameplayTag Channel, const FPayloadHyperMail& Payload)
{
  for (auto& key : Payload.Upgrades)
  {
    OrderedUpgrades.Add(key);
  }
}

void AUpgradeManager::OnEquipmentPickup(FGameplayTag Channel, const FPayloadKey& Payload)
{
  FName Key = Payload.Key;
  if (UItemDataSubsystem::Get(this).ItemHasProperty<FItemPurchaseData>(Key) && !OrderedUpgrades.Contains(Key))
  {
    OrderedUpgrades.Add(Key);
  }
}

//--------------------------TIER---------------------------//

bool AUpgradeManager::IsTierValid(const int32 Tier) const
{
  return Tier <= CurrentTier;
}

bool AUpgradeManager::IsTierPlusOne(const int32 Tier, const bool IncludeLowers) const
{
  if(IncludeLowers)  return Tier <= CurrentTier + 1;
  return Tier == CurrentTier + 1;
 
}

void AUpgradeManager::IncrementTier()
{
  CurrentTier++;
}

int32 AUpgradeManager::GetCurrentTier() const
{
  return CurrentTier;
}

int32 AUpgradeManager::GetCurrentTierPoints() const
{
  return GetTierPoints(CurrentTier);
}

int32 AUpgradeManager::GetTierPoints(int32 Tier) const
{
  return TrialTiersDA->TierPoints[Tier];
}


//------------------------------TRIALS----------------------------//

TMap<FName, int32> AUpgradeManager::GetAvailableRewards(const FName TrialKey, const int32 MaxRewards) const
{
  int32 TrialGroup = UItemDataSubsystem::Get(this).GetItemProperty<FTrialData>(TrialKey).RewardGroup;
  TMap<FName, int32> AllRewards = TrialTiersDA->TrialRewards[TrialGroup].Rewards;
  TMap<FName, int32> AvailableRewards;
  for (auto& reward : AllRewards)
  {
    if (IsTrialRewardAvailable(reward.Key))
    {
      AvailableRewards.Add(reward);
    }
  }

  while (AvailableRewards.Num() > MaxRewards)
  {
    int32 RandIndex = UKismetMathLibrary::RandomIntegerInRange(0, AvailableRewards.GetMaxIndex());
    TArray<FName> keys;
    AvailableRewards.GetKeys(keys);
    AvailableRewards.Remove(keys[RandIndex]);
  }

  return AvailableRewards;
}

bool AUpgradeManager::IsTrialRewardAvailable(const FName Reward) const
{
   return !ClaimedRewards.FindRef(CurrentTier).Rewards.Contains(Reward);
}

bool AUpgradeManager::IsTrialTierValid(const FName TrialKey, int32& MinTier) const
{
  MinTier = UItemDataSubsystem::Get(this).GetItemOrder(TrialKey);
  return MinTier <= CurrentTier;
}

void AUpgradeManager::IncrementTrialPoints(const int32 Amount)
{
  TotalTrialPoints += Amount; 
  UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().Notification, FPayloadNotification(FText::FromString("Trial Points"), FText::FromString(FString("+").Append(FString::FromInt(Amount)))));
 
  int32 count = 0;
  for (int32 i =0; i < TrialTiersDA->TierPoints.Num(); i++)
  {
    count += GetTierPoints(i);
    if (TotalTrialPoints < count)
    {
      if (CurrentTier < i) //If we're in next tier, increment tier and broadcast. Otherwise just return at this point.
      {
        CurrentTier = i;
        UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().TierUp, FPayloadInt(CurrentTier));
      }
      return;
    }
  }
}

int32 AUpgradeManager::GetTrialPointsInCurrentTier() const
{
  int32 count = 0;
  for (int32 i = 0; i < CurrentTier; i++)
  {
    count += GetTierPoints(i);
  }
  return TotalTrialPoints - count;
}

int32 AUpgradeManager::GetTotalTrialPoints() const
{
  return TotalTrialPoints;
}

void AUpgradeManager::OnRewardClaimed(FGameplayTag Channel, const FPayloadKeyInt& Payload)
{
  FName RewardKey = Payload.Key;
  if (!RewardKey.IsNone())
  {
    if (ClaimedRewards.Contains(CurrentTier))
    {
      ClaimedRewards.FindRef(CurrentTier).Rewards.Add(RewardKey);
      return;
    }
    ClaimedRewards.Add(CurrentTier, FTrialRewards(RewardKey));
  }
}

//----------------------------CREDITS-----------------------------//

void AUpgradeManager::UpdateHyperMailCredit(int32 Change)
{
  HyperMailCredits += Change;
}

int32 AUpgradeManager::GetHyperMailCredits() const
{
  return HyperMailCredits;
}


//-------------------------SCRIPT INSTALLS-----------------------//

void AUpgradeManager::Upgrade_Inventory1()
{
  UManagerSubsystem::Get(this).Avatar->Backpack->IncreaseStorageCapacity(6);
}

void AUpgradeManager::Upgrade_Deconstructor()
{
  UManagerSubsystem::Get(this).EquipmentManager->Deconstructor = true;
}

void AUpgradeManager::Upgrade_TBenderCapsule2()
{
  UManagerSubsystem::Get(this).RewindManager->TBenderFilledCapsules++;
  UManagerSubsystem::Get(this).RewindManager->TBenderAvailableCapsules++;
}
