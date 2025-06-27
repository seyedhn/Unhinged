#include "SplineBlock.h"
#include "../Manager/ManagerSettings.h"


ASplineBlock::ASplineBlock()
{
  USceneComponent* DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName(TEXT("DefaultSceneRoot")));
  //DefaultSceneRoot->SetupAttachment(RootComponent);
  RootComponent = DefaultSceneRoot;

  UClass* SplineClass = GetDefault<UManagerSettings>()->SplineComponent.Get();
  SplineComp = Cast<USplineComp>(CreateDefaultSubobject(FName(TEXT("SplineComp")), SplineClass, SplineClass, false, false));
  if(SplineComp) SplineComp->SetupAttachment(DefaultSceneRoot);
  
}

void ASplineBlock::BeginPlay()
{
  Super::BeginPlay();
  if (!bIsSpawnLoaded) Initialise();
}

void ASplineBlock::OnActorSpawnLoaded_Implementation(bool RewindLoad)
{
  Super::OnActorSpawnLoaded_Implementation(RewindLoad);
  Initialise();
  SplineComp->SetEndMeshesOnLoad();
  SplineComp->GenerateNewSpline();
  SplineComp->ActivateSplineCollision();
  Materialize();
}

void ASplineBlock::OnActorReloaded_Implementation(bool RewindLoad)
{
  Super::OnActorReloaded_Implementation(RewindLoad);
  Initialise();
  SplineComp->SetEndMeshesOnLoad();
  SplineComp->GenerateNewSpline();
  SplineComp->ActivateSplineCollision();
  Materialize();
}
