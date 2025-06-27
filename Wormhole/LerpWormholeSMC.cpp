#include "LerpWormholeSMC.h"
#include "../Avatar/Avatar.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "../Messaging/ChannelTags.h"
#include "../Messaging/PayloadStructs.h"


ULerpWormholeSMC::ULerpWormholeSMC()
{
  TeleportTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TeleportTimeline"));
  AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
}

void ULerpWormholeSMC::BeginPlay()
{
  Super::BeginPlay();

  InitialiseTimeline(TeleportTimeline, TeleportCurve, FName("OnTeleportUpdated"), FName("OnTeleportFinished"), 1.f, 1.f);

}


void ULerpWormholeSMC::TeleportCharacter(AActor* Character)
{
  SuckedActor = Character;
  StartLocation = Character->GetActorLocation();
  FVector OtherEnd = OtherWormhole->GetComponentLocation();
  FVector DistanceVec = (OtherEnd - StartLocation).GetSafeNormal() * 300.f;
  ExitLocation = OtherEnd + DistanceVec;// +FVector(0.f, 0.f, 300.f);

  EnableCharacterMovement(Cast<ACharacter>(Character), false);
  TeleportEffect(true);
  SetDistanceBasedPlayRate();
  TeleportTimeline->PlayFromStart();
}

void ULerpWormholeSMC::OnTeleportUpdated(float alpha)
{
  FVector NewLocation = UKismetMathLibrary::VLerp(StartLocation, ExitLocation, alpha);
  SuckedActor->SetActorLocation(NewLocation);
 // UE_LOG(LogTemp, Warning, TEXT("ALPHA: %s"), *FString::SanitizeFloat(alpha));
}

void ULerpWormholeSMC::OnTeleportFinished()
{
  EnableCharacterMovement(Cast<ACharacter>(SuckedActor), true);
  TeleportEffect(false);
  SuckedActor = nullptr; 
}



void ULerpWormholeSMC::EnableCharacterMovement(ACharacter* Character, bool Enable)
{
  if (Enable)
  {
    Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    return;
  }
  Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
  Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ULerpWormholeSMC::TeleportEffect(bool Enable)
{
  UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().PPActivateMaterial, FPayloadKeyBool(FName("Wormhole"), Enable));

  if (Enable)
  {
    USoundBase* TeleportSound = TeleportSoundSoftRef.LoadSynchronous();
    AudioComp->SetSound(TeleportSound);
    AudioComp->FadeIn(0.2f);
    return;
  }
  AudioComp->FadeOut(0.2f, 0.0f);

}

void ULerpWormholeSMC::SetDistanceBasedPlayRate()
{
  TeleportTimeline->SetPlayRate(70.f / UKismetMathLibrary::Sqrt((OtherWormhole->GetComponentLocation() - GetComponentLocation()).Length()));
}






