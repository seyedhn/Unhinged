#include "GridNodeConsumer.h"
#include "../Manager/ManagerSubsystem.h"



void UGridNodeConsumer::PropagateNetwork(UGridNode* NodeBefore)
{
  ConnectToGrid(true);
}

void UGridNodeConsumer::OnMaterialize()
{
  for (UGridNode* neighbour : FindNearbyNodes())
  {
    if (neighbour->IsConnectedToGrid() && neighbour->CanTakeConnection())
    {
      AddNeighbour(neighbour);
      ConnectToGrid();
      if (GetNeighbour()->IsPowered()) OnPowerRestore();
      return; //terminate loop early
    }
  }
}

void UGridNodeConsumer::OnDestroyed(AActor* Actor)
{
  if (IsConnectedToGrid())
  {
    GetNeighbour()->RemoveNeighbour(this);
    UManagerSubsystem::Get(this).GridManager->RemoveNodeFromGrid(GridID, this, -Power);
  }
}

void UGridNodeConsumer::ConnectToGrid(bool ForceGridReset)
{
  GridID = GetNeighbour()->GetGridID();
  UManagerSubsystem::Get(this).GridManager->AddNodeToGrid(GridID, this, -Power, ForceGridReset);
//  if (GetNeighbour()->IsPowered()) OnPowerRestore();
//  else OnPowerOutage();
}

bool UGridNodeConsumer::TryUpdatePower(float NewPower)
{
    if (NewPower == 0.f) //goes back to idle state
    {
        Power = IdlePower;
        return true;
    }

    UManagerSubsystem::Get(this).GridManager->UpdateNodePower(GridID, -Power, -NewPower, false);

    if (!IsPowered()) //In case of power outage
    {
        UManagerSubsystem::Get(this).GridManager->UpdateNodePower(GridID, -NewPower, -Power, true);
        return false;
    }

    Power = NewPower;
    return true;
}

bool UGridNodeConsumer::CanTakeConnection() const
{
  return Connections.Num() == 0;
}