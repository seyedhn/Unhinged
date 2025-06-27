#include "GameActor.h"


AGameActor::AGameActor()
{
  bEnableAutoLODGeneration = false;
}

void AGameActor::OnConstruction(const FTransform& Transform)
{
  ActorID = FGuid::NewGuid();
}


FGuid AGameActor::GetActorID_Implementation() const
{
  return ActorID;
}

bool AGameActor::SaveAllComps_Implementation() const
{
  return true;
}

void AGameActor::SetActorID_Implementation(FGuid SavedActorID)
{
  ActorID = SavedActorID;
}

void AGameActor::OnActorSpawnLoaded_Internal(bool RewindLoad)
{
  bIsSpawnLoaded = true;
}
