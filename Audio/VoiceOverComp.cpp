#include "VoiceOverComp.h"
#include "GameFramework/GameplayMessageSubsystem.h"

void UVoiceOverComp::BeginPlay()
{
  Super::BeginPlay();

  UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);

  MessageSubsystem.RegisterListener(FChannelTags::Get().VoiceOver, this, &UVoiceOverComp::PlayVO);
  MessageSubsystem.RegisterListener(FChannelTags::Get().EquipmentPickup, this, &UVoiceOverComp::PlayVO);
  OnAudioFinished.AddUniqueDynamic(this, &UVoiceOverComp::OnVoiceOverFinished);


}

void UVoiceOverComp::PlayVO(FGameplayTag Channel, const FPayloadKey& Payload)
{
  FName Key = Payload.Key;
  if (!PlayedVOs.Contains(Key) && VoiceOverDA->VO.Contains(Key))
  {
    PlayedVOs.Add(Key);
    CurrentKey = Key;
    if (USoundBase* VOSound = VoiceOverDA->VO.Find(Key)->LoadSynchronous())
    {
      SetSound(VOSound);
      Play();
    }
    
    
  }
}

void UVoiceOverComp::OnVoiceOverFinished()
{
  if (!CurrentKey.IsNone() && VoiceOverDA->VoiceOverToHint.Contains(CurrentKey))
  {
    FName Key = VoiceOverDA->VoiceOverToHint.FindRef(CurrentKey);
    UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().Hint, FPayloadKey(Key));
  }
}

void UVoiceOverComp::PlayRecallVO()
{
  CurrentKey = NAME_None;
  SetSound(RecallVO);
  Play();
}
