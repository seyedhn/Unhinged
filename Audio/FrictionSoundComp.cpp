#include "FrictionSoundComp.h"
#include "Kismet/GameplayStatics.h"
#include "AudioManager.h"

void UFrictionSoundComp::BeginPlay()
{
  Super::BeginPlay();
  SetTimer();
}

void UFrictionSoundComp::Initialise(AAudioManager* _AudioManager, const FFrictionSound& _FrictionSound)
{
  AudioManager = _AudioManager;
  FrictionSound = _FrictionSound;
}

void UFrictionSoundComp::CollisionDetected(float Speed)
{
  SetFloatParameter(TEXT("SlideSpeed"), Speed); //FrictionSound.FrictionSound->
  SetTimer();
}

void UFrictionSoundComp::SetTimer()
{
  GetWorld()->GetTimerManager().SetTimer(StopTimer, this, &UFrictionSoundComp::StopSound, 0.2, false);
}

void UFrictionSoundComp::StopSound()
{
  AudioManager->TerminateFrictionSound(FrictionSound);
  DestroyComponent();
}
