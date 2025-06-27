#include "SceneCaptureCubeComp.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Manager/ManagerSubsystem.h"
#include "../Manager/ManagerSettings.h"


void USceneCaptureCubeComp::SoftInitialise()
{
  GetWorld()->GetTimerManager().SetTimer(ActiveTimer, this, &USceneCaptureCubeComp::RotateTowardsPlayer, 0.02, true);
}

void USceneCaptureCubeComp::Initialise(USceneCaptureCubeComp* Other, float ActiveDistance)
{
  UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
//  MessageSubsystem.RegisterListener(FChannelTags::Get().WormholeCapture, this, &USceneCaptureCubeComp::ListenForCaptures);
 // GEngine->Exec(GetWorld(), TEXT("r.SceneRenderTargetResizeMethod 0"));
  
  ActivationDistance = ActiveDistance;
  OtherSceneCapture = Other;
  HideActorComponents(UGameplayStatics::GetPlayerPawn(this, 0));
  HideActorComponents(GetOwner());
 // CheckIfPlayerIsClose();

  FTimerHandle GreenTexture;
  GetWorld()->GetTimerManager().SetTimer(GreenTexture, this, &USceneCaptureCubeComp::CheckIfTextureIsGreen, FMath::SRand(), false);
  GetWorld()->GetTimerManager().SetTimer(ActiveTimer, this, &USceneCaptureCubeComp::RotateTowardsPlayer, 0.02, true);
  GetWorld()->GetTimerManager().SetTimer(CheckPlayer, this, &USceneCaptureCubeComp::CheckIfPlayerIsClose, 1.f, true);


}

void USceneCaptureCubeComp::Deinitialise()
{
  //GetWorld()->GetTimerManager().ClearTimer(ActiveTimer);
  GetWorld()->GetTimerManager().ClearTimer(CheckPlayer);
  GetWorld()->GetTimerManager().ClearTimer(CaptureTimer);
  bIsSceneDirty = true;
}


void USceneCaptureCubeComp::TryCapture()
{
  if (bIsSceneDirty)
  {
    UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
 // MessageSubsystem.BroadcastMessage(FChannelTags::Get().WormholeCapture, FPayloadEmpty());

    Capture();

    FTimerDelegate SecondCaptureTimer;
    SecondCaptureTimer.BindLambda([this] {OtherSceneCapture->TryCapture(); });
    GetWorld()->GetTimerManager().SetTimerForNextTick(SecondCaptureTimer);
  }
}

void USceneCaptureCubeComp::Capture()
{
  HideActorComponents(UManagerSubsystem::Get(this).EquipmentManager->EquippedWeapon);
  MarkSceneClean();
  CaptureScene();
  UE_LOG(LogTemp, Warning, TEXT("CAPTURE TAKEN"));
}


void USceneCaptureCubeComp::MarkSceneDirty()
{
  bIsSceneDirty = true;
  CaptureTimer.Invalidate();
}

void USceneCaptureCubeComp::MarkSceneClean()
{
  bIsSceneDirty = false;
  GetWorld()->GetTimerManager().SetTimer(CaptureTimer, this, &USceneCaptureCubeComp::MarkSceneDirty, 20, true);
}

void USceneCaptureCubeComp::ListenForCaptures(FGameplayTag Channel, const FPayloadEmpty& Payload)
{
  MarkSceneDirty();
}





void USceneCaptureCubeComp::CheckIfPlayerIsClose()
{
  bool bPlayerInRange = IsPlayerClose();
  if (bPlayerInRange && !bPlayerIsClose)
  {
    bPlayerIsClose = true;
    GetWorld()->GetTimerManager().UnPauseTimer(ActiveTimer);
    TryCapture();
    OnPlayerIsClose.Broadcast(this, true);
    return;
  }
  if (!bPlayerInRange && bPlayerIsClose)
  {
    bPlayerIsClose = false;
    OnPlayerIsClose.Broadcast(this, false);
    GetWorld()->GetTimerManager().PauseTimer(ActiveTimer);
  }
}

bool USceneCaptureCubeComp::IsPlayerClose() const
{
  if (AActor* Player = UManagerSubsystem::Get(this).Avatar)
  {
    return  (Player->GetActorLocation() - GetComponentLocation()).Length() < ActivationDistance;
  }
  return false;
}

void USceneCaptureCubeComp::RotateTowardsPlayer()
{
  SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), UGameplayStatics::GetPlayerCameraManager(this, 0)->GetRootComponent()->GetComponentLocation()));  
}

void USceneCaptureCubeComp::CheckIfTextureIsGreen()
{
  TArray<FLinearColor> pixelColours;
  if (TextureTarget->GameThread_GetRenderTargetResource()->ReadLinearColorPixels(pixelColours, FReadSurfaceDataFlags(RCM_MinMax, CubeFace_MAX), FIntRect(0, 0, 1, 1)))
  {
    if (pixelColours[0] == FLinearColor(0.f, 1.f, 0.f))
    {
      CaptureScene();
    }
  }
}
