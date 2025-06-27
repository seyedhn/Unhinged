#include "DynamicPPComp.h"
#include "PostprocessManager.h"

/*
UDynamicPPComp::UDynamicPPComp()
{
  PPTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("PPTimeline"));
  Curve = NewObject<UCurveFloat>();


}*/

UMaterialInstanceDynamic* UDynamicPPComp::InitialisePPMaterial(FName Key, FPostprocessStruct _PPStruct, UFPCameraComp* Camera)
{
  MaterialKey = Key;
  PPStruct = _PPStruct;
  FPCamera = Camera;
  DynamicPP = UMaterialInstanceDynamic::Create(PPStruct.Material.LoadSynchronous(), this);

  return DynamicPP;

  /*
  Curve->FloatCurve.UpdateOrAddKey(0.f, 0.f);
  Curve->FloatCurve.UpdateOrAddKey(1.f, 1.f);

  FOnTimelineFloat TimelineUpdated;
  FOnTimelineEventStatic TimelineFinished;

  TimelineUpdated.BindUFunction(this, FName("OnTimelineUpdated"));
  TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));

  if (Curve)
  {
    PPTimeline->AddInterpFloat(Curve, TimelineUpdated);
    PPTimeline->SetTimelineFinishedFunc(TimelineFinished);

    PPTimeline->SetTimelineLength(1.f);
    PPTimeline->SetPlayRate(1);
    PPTimeline->SetLooping(false);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("Failed to construct Curve. See RewindComp.cpp"));
  }
  */
  
}

void UDynamicPPComp::InitialisePPSettings(UFPCameraComp* Camera, FPostProcessSettings PPSettings)
{
  FPCamera = Camera;
  Settings = PPSettings;
}

/*
void UDynamicPPComp::ActivatePP()
{
  PPTimeline->PlayFromStart();
}*/



void UDynamicPPComp::RemovePP()
{
//  Cast<APostprocessManager>(GetOwner())->RemovePP(this);
  DestroyComponent();
}

void UDynamicPPComp::OnTimelineUpdated(float alpha)
{
  UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::SanitizeFloat(alpha));
}

void UDynamicPPComp::OnTimelineFinished()
{
  UE_LOG(LogTemp, Warning, TEXT("TIMELINE FINISHED"));
}

