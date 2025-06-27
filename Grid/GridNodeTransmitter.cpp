#include "GridNodeTransmitter.h"
#include "../Manager/ManagerSubsystem.h"


void UGridNodeTransmitter::OnMaterialize()
{
  TArray<FGuid> GridIDs;
  for (UGridNode* neighbour : FindNearbyNodes())
  {
    if (neighbour->IsConnectedToGrid())
    {
      GridIDs.AddUnique(neighbour->GetGridID());
    }
  }
  UManagerSubsystem::Get(this).GridManager->ReEstablishGrids(GridIDs);
}

void UGridNodeTransmitter::OnDestroyed(AActor* Actor)
{
  if (IsConnectedToGrid())
  {
    MarkAsGarbage();
    for (UGridNode* neighbour : FindNearbyNodes())
    {
      neighbour->OnPowerOutage();
    }
   // UManagerSubsystem::Get(this).GridManager->RemoveNodeFromGrid(GridID, this, -Power);
    UManagerSubsystem::Get(this).GridManager->ReEstablishGrid(GridID);
  }
}


void UGridNodeTransmitter::PropagateNetwork(UGridNode* NodeBefore)
{
  GridID = NodeBefore->GetGridID();
  ConnectToGrid();

  for (UGridNode* node : FindNearbyNodes())
  {
    if (!node->IsConnectedToGrid())
    {
      AddNeighbour(node);
      node->PropagateNetwork(this);
    }
  }
}

void UGridNodeTransmitter::ConnectToGrid(bool ForceGridReset)
{
  UManagerSubsystem::Get(this).GridManager->AddNodeToGrid(GridID, this, -Power);
}


bool UGridNodeTransmitter::CanTakeConnection() const
{
  return true; //we can impose limit on number of transmitter connections here.
}