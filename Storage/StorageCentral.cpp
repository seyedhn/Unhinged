#include "StorageCentral.h"
#include "../Messaging/ChannelTags.h"
#include "GameFramework/GameplayMessageSubsystem.h"


void AStorageCentral::BeginPlay()
{
  UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);

  MessageSubsystem.RegisterListener(FChannelTags::Get().StorageNetwork, this, &ThisClass::EstablishNetwork);
}

void AStorageCentral::AddPoleToCentral(AStorageBase* Pole)
{
  AllPoles.Add(Pole);
}

void AStorageCentral::EstablishNetwork(FGameplayTag channel, const FEmptyPayload& payload)
{
  ResetNetwork();
  PropagateNetwork();
}

void AStorageCentral::ResetNetwork()
{
  for (auto& i : AllPoles)
  {
    i->ConnectToCentral(false);
  }
  AllPoles.Empty();
}

bool AStorageCentral::IsWithinStorageRange(AActor* TargetActor, float RangeSquared) const
{
  FVector location = TargetActor->GetActorLocation();
  if ((location - this->GetActorLocation()).SquaredLength() < RangeSquared)
  {
    return true;
  }

  for (auto& i : AllPoles)
  {
    if (i->IsConnected() && (location - i->GetActorLocation()).SquaredLength() < RangeSquared)
    {
      return true;
    }
  }
  return false;


}
