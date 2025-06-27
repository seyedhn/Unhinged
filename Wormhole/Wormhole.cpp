#include "Wormhole.h"
#include "../Manager/ManagerSettings.h"
#include "../Manager/ManagerSubsystem.h"


AWormhole::AWormhole()
{
  USceneComponent* DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName(TEXT("DefaultSceneRoot")));
  RootComponent = DefaultSceneRoot;

  SceneCapture = CreateDefaultSubobject<USceneCaptureCubeComp>(FName(TEXT("SceneCapture")), false);
  SceneCapture->SetupAttachment(DefaultSceneRoot);

  WormholeMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("WormholeMesh")), false);
  WormholeMesh->SetupAttachment(SceneCapture);

  SceneCapture2 = CreateDefaultSubobject<USceneCaptureCubeComp>(FName(TEXT("SceneCapture2")), false);
  SceneCapture2->SetupAttachment(DefaultSceneRoot);

  WormholeMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("WormholeMesh2")), false);
  WormholeMesh2->SetupAttachment(SceneCapture2);




}

bool AWormhole::SaveAllComps_Implementation() const
{
    return false;
}

void AWormhole::WormholeSucked()
{
  if (!bIsLive)
  {
    UManagerSubsystem::Get(this).Avatar->EquipmentHandler->DynamicWormhole = nullptr;
  }
  Destroy();

}
