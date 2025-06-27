#include "StorageBase.h"
#include "../Manager/ManagerSubsystem.h"
#include "../Manager/ManagerSettings.h"



void AStorageBase::AddConnection(AStorageBase* Other, UActorComponent* VFX)
{
  ImmediateConnections.Add(TTuple<AStorageBase*, UActorComponent*>(Other, VFX));

}

void AStorageBase::Disconnect()
{
  for (auto& i : ImmediateConnections)
  {
    i.Get<1>()->DestroyComponent();
    i.Get<0>()->ImmediateConnections.Remove(TTuple<AStorageBase*, UActorComponent*>(this, i.Get<1>()));
  }
  ImmediateConnections.Empty();
}

bool AStorageBase::IsConnected()
{
  return ConnectedToCentral;
}

void AStorageBase::PropagateNetwork()
{
  for (auto& i : ImmediateConnections)
  {
    if (!i.Get<0>()->ConnectedToCentral)
    {
      i.Get<0>()->ConnectToCentral(true);
      i.Get<0>()->PropagateNetwork();
    }
  }
}


void AStorageBase::ConnectToCentral_Implementation(bool Connect)
{
  ConnectedToCentral = Connect;
  if (Connect)
  {
    UManagerSubsystem::Get(this).CentralStorage->AddPoleToCentral(this);
  }
}
