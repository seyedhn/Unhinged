#include "WormholeSMCBase.h"
#include "../Avatar/Avatar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


UWormholeSMCBase::UWormholeSMCBase()
{
  PrimaryComponentTick.bCanEverTick = false;
  CompSuctionTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CompSuctionTimeline"));
}

void UWormholeSMCBase::BeginPlay()
{
  Super::BeginPlay();
  SetOtherWormhole();
  OnComponentBeginOverlap.AddUniqueDynamic(this, &UWormholeSMCBase::OnOverlap);
  InitialiseTimeline(CompSuctionTimeline, SuctionCurve, FName("OnCompSuctionUpdated"), FName("OnCompSuctionFinished"), 0.2f, 2.f);
}

void UWormholeSMCBase::ActivateWormholeSMC(bool Activate)
{
  ECollisionEnabled::Type collision = Activate ? ECollisionEnabled::Type::QueryOnly : ECollisionEnabled::Type::NoCollision;
  SetCollisionEnabled(collision);
}

void UWormholeSMCBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (!SuckedActor)
  {
    if (OtherActor->GetClass()->IsChildOf(AAvatar::StaticClass()))
    {
      TeleportCharacter(OtherActor);
    }
    else if (OtherActor->GetClass()->IsChildOf(APawn::StaticClass()))
    {
      TeleportComponent(Cast<UPrimitiveComponent>(OtherActor->GetRootComponent()));
    }
  }

  if (!SuckedComp && OtherComp->IsSimulatingPhysics())
  {
    TeleportComponent(OtherComp);
  }

}



void UWormholeSMCBase::TeleportComponent(UPrimitiveComponent* Comp)
{
  //RELEASE COMPONENT ON QGUN
  SuckedComp = Comp;
  StartLocation = Comp->GetComponentLocation();
  TargetLocation = GetComponentLocation();

  FVector LengthVec = (GetComponentLocation() - StartLocation);
  ExitLocation = OtherWormhole->GetComponentLocation() + FVector(LengthVec.X, LengthVec.Y, 0.f).GetSafeNormal() * 100.f;

  Comp->SetCollisionResponseToChannel(GetCollisionObjectType(), ECollisionResponse::ECR_Ignore);
  UGameplayStatics::PlaySoundAtLocation(this, SuctionSoundSoftRef.LoadSynchronous(), GetComponentLocation());

  CompSuctionTimeline->PlayFromStart();
}

void UWormholeSMCBase::OnCompSuctionUpdated(float alpha)
{
  FVector NewLocation = UKismetMathLibrary::VLerp(StartLocation, TargetLocation, alpha);
  SuckedComp->SetWorldLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
}

void UWormholeSMCBase::OnCompSuctionFinished()
{
  ActivateCompCollisionResponseOnTimer(SuckedComp);
  SuckedComp->SetWorldLocation(ExitLocation, false, nullptr, ETeleportType::TeleportPhysics);
  SuckedComp = nullptr;
}




void UWormholeSMCBase::SetOtherWormhole()
{
  TArray<UWormholeSMCBase*> wormholes;
  GetOwner()->GetComponents<UWormholeSMCBase>(wormholes);
  for (UWormholeSMCBase* wormhole : wormholes)
  {
    if (wormhole != this)
    {
      OtherWormhole = wormhole;
      break;
    }
  }
}


void UWormholeSMCBase::InitialiseTimeline(UTimelineComponent* Timeline, UCurveFloat* Curve, FName UpdateFuncName, FName FinishedFuncName, float TimelineLength, float PlayRate)
{
  FOnTimelineFloat TimelineUpdated;
  FOnTimelineEventStatic TimelineFinished;

  TimelineUpdated.BindUFunction(this, UpdateFuncName);
  TimelineFinished.BindUFunction(this, FinishedFuncName);

  if (Curve)
  {
    Timeline->AddInterpFloat(Curve, TimelineUpdated);
    Timeline->SetTimelineFinishedFunc(TimelineFinished);

    Timeline->SetTimelineLength(TimelineLength);
    Timeline->SetLooping(false);
    Timeline->SetPlayRate(PlayRate);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("Failed to construct Curve. See WormholeSMC.cpp"));
  }
}


void UWormholeSMCBase::ActivateCompCollisionResponseOnTimer(UPrimitiveComponent* comp)
{
  FTimerHandle Timer;
  FTimerDelegate Delegate;
  Delegate.BindLambda([this, comp] { comp->SetCollisionResponseToChannel(GetCollisionObjectType(), ECollisionResponse::ECR_Overlap); });
  GetWorld()->GetTimerManager().SetTimer(Timer, Delegate, 0.5f, false);
}





