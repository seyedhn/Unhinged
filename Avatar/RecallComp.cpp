#include "RecallComp.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/GameplayMessageSubsystem.h"



void URecallComp::BeginPlay()
{

  Super::BeginPlay();

  SetStampTimer();

  UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);

  MessageSubsystem.RegisterListener(FChannelTags::Get().GameSaved, this, &ThisClass::ClearTimestamps);

}


void URecallComp::RecordTimestamps()
{
  if (StampIndex == StampLength-1)
  {
    ResizeTimestampArray();
  }

  if (GetOwner()->GetVelocity().SquaredLength() > 1.f)
  {
    Timestamps[StampIndex] = GetOwner()->GetActorLocation();
    StampIndex++;
  }
  
}


void URecallComp::ResizeTimestampArray()
{
  for (int i = 0; i < StampLength-1; i+=2)
  {
   // Timestamps[i/2] = ComputeAverageLocation(Timestamps[i], Timestamps[i + 1]);
    Timestamps[i/2] = Timestamps[i];
  }

  StampIndex = StampLength/2;
  TimerRate *= 2;
  SetStampTimer();
}


FVector URecallComp::ComputeAverageLocation(FVector loc1, FVector loc2)
{
  return (loc1 + loc2) / 2;
}


void URecallComp::BeginRecall()
{
  GetWorld()->GetTimerManager().SetTimer(RecallTimer, this, &URecallComp::ExecuteRecall, 0.02, true);
  GetWorld()->GetTimerManager().ClearTimer(StampTimer);

  RecallAudioComp = UGameplayStatics::SpawnSound2D(this, RecallSound);
  RecallAudioComp->SetUISound(false);
}

void URecallComp::ExecuteRecall()
{
  if (StampIndex == 0) 
  {
    EndRecall();
    return;
  }

  GetOwner()->SetActorLocation(Timestamps[StampIndex]);
  StampIndex--;
  if (UKismetMathLibrary::RandomBoolWithWeight(0.1)) UGameplayStatics::SpawnSound2D(this, RecallWhooshSound)->SetUISound(false);
  //UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::FromInt(StampIndex) );
}

void URecallComp::EndRecall()
{
  StampIndex = 0;
  GetWorld()->GetTimerManager().ClearTimer(RecallTimer);
  SetStampTimer();
  OnRecallCompleted.Broadcast();
  RecallAudioComp->SetTriggerParameter(TEXT("Finished"));
}


void URecallComp::SetStampTimer()
{
  GetWorld()->GetTimerManager().SetTimer(StampTimer, this, &URecallComp::RecordTimestamps, TimerRate, true);
}

void URecallComp::ClearTimestamps(FGameplayTag Channel, const FPayloadEmpty& Payload)
{
  StampIndex = 0;
  TimerRate = 0.1;
}

















/*
FTransform URecallComp::ComputeAverageTransform(FTransform transform1, FTransform transform2)
{
  FVector avgloc = (transform1.GetLocation() + transform2.GetLocation()) / 2;
  FQuat avgrot = FQuat::Slerp(transform1.GetRotation(), transform2.GetRotation(), 0.5);

  return FTransform(avgrot, avgloc, FVector(1.f));
}*/