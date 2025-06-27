#include "DuctFlowComponent.h"
#include "../Block/BlockActor.h"
#include "../Block/BlockSocket.h"
#include "Kismet/GameplayStatics.h"


void UDuctFlowComponent::Initialise(USplineComponent* spline)
{
  Spline = spline;
}

void UDuctFlowComponent::ObjectFlows(FName object, ABlockActor* PreviousBlock)
{
  FDuctFlow DuctFlow = FDuctFlow(object, PreviousBlock);
  float TimerLength = Spline->GetSplineLength() / 500;
  PlayStartAndMiddleSounds(TimerLength);
  GetWorld()->GetTimerManager().SetTimer(DuctFlow.Timer, this, &UDuctFlowComponent::ExitObject, TimerLength, false);
  Queue.Enqueue(DuctFlow);
}

void UDuctFlowComponent::ExitObject()
{
  FDuctFlow DuctFlow = *Queue.Peek();
  Cast<ABlockActor>(GetOwner())->FlowObjectToNextBlock(DuctFlow.PreviousBlock, DuctFlow.Object, EBlockSocketType::Conveyor);
  GetWorld()->GetTimerManager().ClearTimer(DuctFlow.Timer);
  UGameplayStatics::PlaySoundAtLocation(this, ProcessSound, Spline->GetLocationAtSplinePoint(Spline->GetNumberOfSplinePoints()-1, ESplineCoordinateSpace::World));
  Queue.Dequeue();
}


void UDuctFlowComponent::PlayStartAndMiddleSounds(float TimerLength)
{
  UGameplayStatics::PlaySoundAtLocation(this, ProcessSound, Spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World));
  FTimerHandle SoundTimer;
  FTimerDelegate SoundDelegate;
  SoundDelegate.BindLambda([this] {UGameplayStatics::PlaySoundAtLocation(this, ProcessSound, Spline->GetLocationAtSplinePoint(Spline->GetNumberOfSplinePoints()/2, ESplineCoordinateSpace::World)); });
  GetWorld()->GetTimerManager().SetTimer(SoundTimer, SoundDelegate, TimerLength / 2.f, false);
}