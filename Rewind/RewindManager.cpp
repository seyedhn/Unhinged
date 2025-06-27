#include "RewindManager.h"
#include "Kismet/GameplayStatics.h"
#include "../Manager/ManagerSettings.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "../Messaging/PayloadStructs.h"
#include "../Messaging/ChannelTags.h"




void ARewindManager::BeginPlay()
{
  Super::BeginPlay();

  RewindSound = GetDefault<UManagerSettings>()->RewindSound.LoadSynchronous();
  RewindEndSound = GetDefault<UManagerSettings>()->RewindEndSound.LoadSynchronous();
  RewindSoundMix = GetDefault<UManagerSettings>()->RewindSoundMix.LoadSynchronous();
}

TArray<URewindComp*> ARewindManager::GetActiveRewindComps() const
{
  return ActiveRewindComps;
}

bool ARewindManager::BeginFullRewind()
{
  if (DormantRewindComps.Num() == 0)
  {
    return false;
  }
  bFullRewind = true;
  ActivateRewindPP(true);
  for (auto& comp : DormantRewindComps)
  {
    comp->BeginRewind();
  }
  return true;
}


void ARewindManager::OnRewindBegin(URewindComp* RewindComp)
{
  if (!IsRewinding())
  {
    ActivateRewindPP(true);
   RewindSoundComp = UGameplayStatics::SpawnSound2D(this, RewindSound);
   RewindSoundComp->SetUISound(false);
   UGameplayStatics::PushSoundMixModifier(this, RewindSoundMix);
  }
  ActiveRewindComps.Add(RewindComp);
  RewindComp->OnRewindEnd.AddUniqueDynamic(this, &ARewindManager::OnRewindEnd);
}

void ARewindManager::CancelAllRewinds()
{
  UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::FromInt(ActiveRewindComps.Num()));
  if (IsRewinding())
  {
    while (IsRewinding() && ActiveRewindComps.Top())
    {
      ActiveRewindComps.Top()->CancelRewind();
    }
    /*
    ActiveRewindComps.Empty();
    bFullRewind = false;
    ActivateRewindPP(false);
    UGameplayStatics::PlaySound2D(this, RewindEndSound);
    RewindSoundComp->SetTriggerParameter(TEXT("stopDrone"));
    UGameplayStatics::PopSoundMixModifier(this, RewindSoundMix);*/
  }
}

void ARewindManager::OnRewindEnd(URewindComp* RewindComp)
{
  ActiveRewindComps.Remove(RewindComp);
  UGameplayStatics::PlaySound2D(this, RewindEndSound, 1.f, 1.f, 0.f, nullptr, nullptr, false);
  if (!IsRewinding())
  {
    OnRewindEnded.Broadcast();
    bFullRewind = false;
    ActivateRewindPP(false);
    RewindSoundComp->SetTriggerParameter(TEXT("stopDrone"));
    UGameplayStatics::PopSoundMixModifier(this, RewindSoundMix);
  }
}

bool ARewindManager::IsRewinding() const
{
  return ActiveRewindComps.Num() != 0;
}

bool ARewindManager::IsFullRewinding() const
{
  return bFullRewind;
}

URewindComp* ARewindManager::GetLongestRewindComp() const
{
  if (ActiveRewindComps.Num() == 0) return nullptr;
  TSortedMap<int32, URewindComp*> SortMap;
  for (auto& comp : ActiveRewindComps)
  {
    SortMap.Add(comp->GetTimestampLength(), comp);
  }
  TArray<URewindComp*> comps;
  SortMap.GenerateValueArray(comps);
  return comps.Top();
}

void ARewindManager::OnRewindAdded(URewindComp* RewindComp)
{
  DormantRewindComps.Add(RewindComp);
}

void ARewindManager::OnRewindRemoved(URewindComp* RewindComp)
{
  DormantRewindComps.Remove(RewindComp);
}

void ARewindManager::ActivateRewindPP(bool Activate) const
{
  UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().PPActivateSettings, FPayloadKeyBool(FName(TEXT("BlockRewind")), Activate));
}


