#include "InstantWormholeSMC.h"
#include "../Avatar/Avatar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


UInstantWormholeSMC::UInstantWormholeSMC()
{
  SuctionTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SuctionTimeline"));
}

void UInstantWormholeSMC::BeginPlay()
{
  Super::BeginPlay();

  InitialiseTimeline(SuctionTimeline, SuctionCurve, FName("OnSuctionUpdated"), FName("OnSuctionFinished"), 0.2f, 1.f);

}


void UInstantWormholeSMC::TeleportCharacter(AActor* Character)
{
  SuckedActor = Character;
  //FVector CameraDirection = SuckedActor->GetComponentByClass<UFPCameraComp>()->GetForwardVector(); 
  StartLocation = SuckedActor->GetActorLocation();

  FVector OffsetVector = FVector(0.f, 0.f, -61.f);
  TargetLocation = GetComponentLocation() + OffsetVector;
  
  FVector CameraDirection = (GetComponentLocation() - StartLocation);
  ExitLocation = OtherWormhole->GetComponentLocation() + OffsetVector + FVector(CameraDirection.X, CameraDirection.Y, 0.f).GetSafeNormal() * 100.f;
  UGameplayStatics::PlaySound2D(this, TeleportSoundSoftRef.LoadSynchronous(), 1.f, 1.f, 0.f, nullptr, nullptr, false);
  SuctionTimeline->PlayFromStart();
}

void UInstantWormholeSMC::OnSuctionUpdated(float alpha)
{
  FVector NewLocation = UKismetMathLibrary::VLerp(StartLocation, TargetLocation, alpha);
  SuckedActor->SetActorLocation(NewLocation);
//  UE_LOG(LogTemp, Warning, TEXT("ALPHA: %s"), *FString::SanitizeFloat(alpha));
}

void UInstantWormholeSMC::OnSuctionFinished()
{
  SuckedActor->SetActorLocation(ExitLocation);
  OnCharacterTeleported.Broadcast();
  SuckedActor = nullptr; 
}










