#include "UserSettingsActor.h"
#include "AudioDevice.h"


void AUserSettingsActor::SetSoundVolume(class USoundMix* InSoundMixModifier, class USoundClass* InSoundClass, float Volume)
{
  if (FAudioDeviceHandle AudioDevice = GetWorld()->GetAudioDevice())
  {
    AudioDevice->SetSoundMixClassOverride(InSoundMixModifier, InSoundClass, Volume, 1.f, 1.f, true);
    AudioDevice->PushSoundMixModifier(InSoundMixModifier);
  }
}