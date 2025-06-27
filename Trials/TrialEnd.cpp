#include "TrialEnd.h"
#include "../Resource/ResourceLibrary.h"
#include "../Manager/ManagerSubsystem.h"

void ATrialEnd::BindToGizmo()
{
  UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
  Handle = MessageSubsystem.RegisterListener(FChannelTags::Get().Trial, this, &ThisClass::OnRewardSelected);
}

void ATrialEnd::UnbindFromGizmo()
{
  UGameplayMessageSubsystem::Get(this).UnregisterListener(Handle);
}

void ATrialEnd::OnRewardSelected(FGameplayTag Channel, const FPayloadKeyInt& Payload)
{
  RewardKey = Payload.Key;
  RewardAmount = Payload.amount;
  if (!RewardKey.IsNone())
  {
    DeliverReward();
  }
  UnbindFromGizmo();
}

void ATrialEnd::TryCollectDelivery()
{
  if (UResourceLibrary::TryCollectDelivery(this, RewardKey, RewardAmount))
  {
    DeliveryCollected();
  }
}
