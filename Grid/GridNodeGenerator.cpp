#include "GridNodeGenerator.h"
#include "../Manager/ManagerSubsystem.h"

void UGridNodeGenerator::BeginPlay()
{
  Super::BeginPlay();
  if (GetOwner()->IsActorBeginningPlayFromLevelStreaming())
  {
    InitiatePropagationOnLevelLoad();
    //FTimerHandle Timer;
   // GetWorld()->GetTimerManager().SetTimer(Timer, this, &UGridNodeGenerator::InitiatePropagationOnLevelLoad, 1.f, false);
  }

}

void UGridNodeGenerator::InitiatePropagationOnLevelLoad()
{
  if (!IsConnectedToGrid())  //Run this code only at Level BeginPlay(). The case of spawning is dealt with at OnMaterialize().
  {
    PropagateNetwork(nullptr);
  }  
}

void UGridNodeGenerator::OnMaterialize()
{
  for (UGridNode* neighbour : FindNearbyNodes())
  {
    if (neighbour->CanTakeConnection())
    {
      AddNeighbour(neighbour);

      if (neighbour->IsConnectedToGrid())
      {
        ConnectToGrid();
      }
      else
      {
        MakeGrid();
        neighbour->PropagateNetwork(this);
      }
      return;
    }
  }
  MakeGrid();
}

void UGridNodeGenerator::OnDestroyed(AActor* Actor)
{
  if (Connections.Num() > 0)
  {
    UManagerSubsystem::Get(this).GridManager->RemoveSourceFromGrid(GridID, this, Power);
  }
}


void UGridNodeGenerator::PropagateNetwork(UGridNode* NodeBefore)
{
  if (NodeBefore)
  {
    ConnectToGrid(true); //When Generator is the endpoint in propagation
    return;
  }

  MakeGrid();
  /*
  if (UGridNode* neighbour = FindNearestFreeNode())
  {
    AddNeighbour(neighbour);
    neighbour->PropagateNetwork(this);
  }
  */
  for (UGridNode* node : FindNearbyNodes())
  {
    if (!node->IsConnectedToGrid())
    {
      AddNeighbour(node);
      node->PropagateNetwork(this);
    }
  }
}

void UGridNodeGenerator::ConnectToGrid(bool ForceGridReset)
{
  //this assumes the Generator is the endpoint, and the neighbour is already connected to a grid
  GridID = GetNeighbour()->GetGridID();
  UManagerSubsystem::Get(this).GridManager->AddNodeToGrid(GridID, this, Power, ForceGridReset);
}

void UGridNodeGenerator::MakeGrid()
{
  UManagerSubsystem::Get(this).GridManager->MakeNewGrid(this);
}

bool UGridNodeGenerator::CanTakeConnection() const
{
  return Connections.Num() == 0;
}