#include "RewindComp.h"
#include "RewindInterface.h"
#include "../Manager/ManagerSettings.h"
#include "../Manager/ManagerSubsystem.h"





URewindComp::URewindComp()
{
  PrimaryComponentTick.bCanEverTick = false;
  ConstructTimeline();
  ConstructSpline();
}

void URewindComp::BeginPlay()
{
  Super::BeginPlay();
  Initialise_Internal();
}

void URewindComp::InitialiseRewindComp(UObject* Owner, UPrimitiveComponent* RewindComp)
{
  RewindOwner = Owner;
  RewindableComp = RewindComp;
  Initialise_Internal();
}

void URewindComp::Initialise_Internal()
{
  if (RewindOwner && IRewindInterface::Execute_IsRewindable(RewindOwner)) // Optimisation: Do not record timestamps of non-rewindable, static blocks.
  {
    UManagerSubsystem::Get(this).RewindManager->OnRewindAdded(this); //Need to make sure this broadcast is made on Load Game
    Timestamps.Reserve(StampLength);
    GetWorld()->GetTimerManager().SetTimer(StampTimer, this, &URewindComp::RecordTimestamps, TimerRate, true);
    InitialiseTimeline();
    Spline->ClearSplinePoints(true); // This is to get rid of the default initial 2 spline points 
  }
}


void URewindComp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
  UManagerSubsystem::Get(this).RewindManager->OnRewindRemoved(this);
}




void URewindComp::RecordTimestamps()
{
  if (RewindableComp->GetComponentVelocity().SquaredLength() > 1.f || RewindableComp->GetPhysicsAngularVelocityInDegrees().SquaredLength() > 1.f)
  {
    if (Timestamps.Num() == StampLength)
    {
      Timestamps.PopFront();
    }
    Timestamps.Add(RewindableComp->GetComponentTransform());
  }

}





/// /////REWIND///////////////////


void URewindComp::BeginRewind()
{
  RewindLength = Timestamps.Num();
  IsSimulatingPhysics = RewindableComp->IsSimulatingPhysics();
  UManagerSubsystem::Get(this).RewindManager->OnRewindBegin(this);

  if (RewindLength >= 2)
  {
    GetWorld()->GetTimerManager().ClearTimer(StampTimer);
    if(IsSimulatingPhysics) RewindableComp->SetSimulatePhysics(false);
    MakeSpline();
    bIsRewinding = true;
    RewindTimeline->SetPlayRate(1);
    RewindTimeline->PlayFromStart();
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("Timestamp was empty. Rewind did not initiate"));
    //This is to provide visual feedback that rewind tried to initiate but terminated early
    FTimerHandle EndPPHandle;
    FTimerDelegate EndPPDelegate;
    EndPPDelegate.BindLambda([this] {OnRewindEnd.Broadcast(this); });
    GetWorld()->GetTimerManager().SetTimer(EndPPHandle, EndPPDelegate, 0.2, false);
    
  }

}

void URewindComp::LerpTransforms(float alpha)
{
//  UE_LOG(LogTemp, Warning, TEXT("%s, %s"), *FString::SanitizeFloat(alpha), *FString::FromInt(Timestamps.Num()));
  FTransform transf;
  transf.Blend(Timestamps[Timestamps.Num()-1], Timestamps[Timestamps.Num() - 2], alpha);
  RewindableComp->SetWorldTransform(transf, false, nullptr, ETeleportType::TeleportPhysics);
}

void URewindComp::TraverseTimestamp()
{
  if (Timestamps.Num() <= 2)
  {
    EndRewind();
    return;
  }
  OnRewindProgressed.Broadcast(1 - float(Timestamps.Num()) / float(RewindLength));
  Timestamps.Pop();
  RewindTimeline->PlayFromStart();
}

void URewindComp::EndRewind()
{
  Timestamps.Reset();
  GetWorld()->GetTimerManager().SetTimer(StampTimer, this, &URewindComp::RecordTimestamps, TimerRate, true);
  DestroySpline();
  if(IsSimulatingPhysics) RewindableComp->SetSimulatePhysics(true);
  bIsRewinding = false;
  OnRewindEnd.Broadcast(this);
}

void URewindComp::PauseRewind()
{
  RewindTimeline->Stop();
}

void URewindComp::ResumeRewind()
{
  RewindTimeline->Play();
  RewindTimeline->SetPlayRate(1);
}

void URewindComp::FastRewind()
{
  RewindTimeline->Play();
  RewindTimeline->SetPlayRate(2);
}

void URewindComp::ShiftRewindSpeed(int32 Direction)
{
  float PlayRate = RewindTimeline->GetPlayRate();
  float Factor = (Direction == 1) ? 1.5 : 0.6667;
  float Shift = FMath::Clamp(PlayRate * Factor, 0.13172, 3.375);
  RewindTimeline->SetPlayRate(Shift);
}

void URewindComp::CancelRewind()
{
  if (bIsRewinding)
  {
    RewindTimeline->Stop();
    EndRewind();
  }
}

bool URewindComp::IsRewinding() const
{
  return bIsRewinding;
}

int32 URewindComp::GetTimestampLength() const
{
  return Timestamps.Num();
}

void URewindComp::OnTimelineUpdated(float alpha)
{
  LerpTransforms(alpha);
}

void URewindComp::OnTimelineFinished()
{
  TraverseTimestamp();
}


/////////////////////SPLINE////////////////////////////


void URewindComp::MakeSpline()
{
  for (int i = 0; i < Timestamps.Num(); ++i)
  {
   Spline->AddSplinePoint(Timestamps[i].GetLocation(), ESplineCoordinateSpace::World, true);
  }
  
  UStaticMesh* StaticMesh = GetDefault<UManagerSettings>()->BlockRewindStaticMesh.LoadSynchronous();
  for (int i = 0; i < Spline->GetNumberOfSplinePoints(); ++i)
  {
    USplineMeshComponent* SplineComp = Cast<USplineMeshComponent>(GetOwner()->AddComponentByClass(USplineMeshComponent::StaticClass(), true, FTransform(), false));
    Spline->SetSplinePointType(i, ESplinePointType::Curve, false);
    SplineComp->SetMobility(EComponentMobility::Stationary);
    SplineComp->SetStaticMesh(StaticMesh);
    SplineComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SplineComp->SetForwardAxis(ESplineMeshAxis::Z);
    SplineComp->SetStartAndEnd(
      Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local),
      Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local),
      Spline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local),
      Spline->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local));
    SplineComps.Add(SplineComp);
  }
  

}

void URewindComp::DestroySpline()
{
  Spline->ClearSplinePoints(true);
  if (SplineComps.Num() > 0)
  {
    TArray<UActorComponent*> comps(MoveTemp(SplineComps));
    for (auto& comp : comps)
    {
      if (comp) comp->DestroyComponent();
    }
  }
}


///////////////TIMELINE/////////////////////////

void URewindComp::ConstructTimeline()
{
  RewindTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RewindTimeline"));
}

void URewindComp::ConstructSpline()
{
  Spline = CreateDefaultSubobject<USplineComponent>(TEXT("RewindSpline"));
}

void URewindComp::InitialiseTimeline()
{
  FOnTimelineFloat TimelineUpdated;
  FOnTimelineEventStatic TimelineFinished;

  TimelineUpdated.BindUFunction(this, FName("OnTimelineUpdated"));
  TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));

  UCurveFloat* Curve = NewObject<UCurveFloat>();
  Curve->FloatCurve.UpdateOrAddKey(0.f, 0.f);
  Curve->FloatCurve.UpdateOrAddKey(TimerRate, 1.f);

  if (Curve)
  {
    RewindTimeline->AddInterpFloat(Curve, TimelineUpdated);
    RewindTimeline->SetTimelineFinishedFunc(TimelineFinished);

    RewindTimeline->SetTimelineLength(1.f);
    RewindTimeline->SetLooping(false);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("Failed to construct Curve. See RewindComp.cpp"));
  }
}





