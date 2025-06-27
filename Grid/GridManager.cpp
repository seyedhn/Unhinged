#include "GridManager.h"
#include "../Messaging/ChannelTags.h"
#include "GameFramework/GameplayMessageSubsystem.h"



void AGridManager::MakeNewGrid(UGridNode* Generator)
{
  FGuid GridID = FGuid::NewGuid();
  Generator->GridID = GridID;
  Grids.Add(FGrid(GridID, Generator, Generator->Power));
}

//We need to force grid restart on the initial full propagation of the grid, to ensure if the grid is overloaded, it broadcasts outages.
//This is done on all consumer nodes during propagation.
void AGridManager::AddNodeToGrid(FGuid GridID, UGridNode* Node, float Power, bool ForcePowerCheck)
{
  FGrid* grid = Grids.Find(GridID);
  grid->AddNode(Node);
  grid->UpdatePower(Power);

  FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([GridID, Power, ForcePowerCheck, this] {CheckGridPower(GridID, ForcePowerCheck); });
  GetWorld()->GetTimerManager().SetTimer(grid->TimerHandle, TimerDelegate, 1.f, false); 
}

void AGridManager::UpdateNodePower(FGuid GridID, float OldPower, float NewPower, bool DelayedPowerCheck)
{
    FGrid* grid = Grids.Find(GridID);
    grid->UpdatePower(NewPower-OldPower); //this must be negative because it's a consumer
    if (DelayedPowerCheck)
    {
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([GridID, this] {CheckGridPower(GridID, true); });
        GetWorld()->GetTimerManager().SetTimer(grid->TimerHandle, TimerDelegate, 5.f, false);  //try restore grid power after 5 seconds
        return;
    }
    CheckGridPower(GridID, true);

}

void AGridManager::CheckGridPower(const FGuid& GridID, bool ForceCheck)
{
  FGrid* grid = Grids.Find(GridID);
  grid->CheckPower(ForceCheck);
}

void AGridManager::RemoveNodeFromGrid(FGuid GridID, UGridNode* Node, float Power)
{
  FGrid* grid = Grids.Find(GridID);
  grid->RemoveNode(Node);
  grid->UpdatePower(Power, false);
  grid->CheckPower();
}



void AGridManager::RemoveSourceFromGrid(FGuid GridID, UGridNode* Node, float Power)
{
  FGrid* grid = Grids.Find(GridID);

  grid->RemoveNode(Node);

  if (grid->NoSourceLeft())
  {
    ClearGrid(grid);
    return;
  }
  Node->GetNeighbour()->RemoveNeighbour(Node);
  grid->UpdatePower(Power, false);
}


void AGridManager::ReEstablishGrid(const FGuid& GridID)
{
  FGrid* grid = Grids.Find(GridID);
  TArray<UGridNode*> generators = grid->GetGenerators();
  ClearGrid(grid);
  PropagateGenerators(generators);
}

void AGridManager::ReEstablishGrids(const TArray<FGuid>& GridIDs)
{
  TArray<UGridNode*> generators;
  for (FGuid gridID : GridIDs)
  {
    if (FGrid* grid = Grids.Find(gridID))
    {
      generators.Append(grid->GetGenerators());
      ClearGrid(grid);
    }
  }
  PropagateGenerators(generators);
}


void AGridManager::ClearGrid(FGrid* Grid)
{
  Grid->Reset();
  Grids.Remove(Grid->GridID);
}


void AGridManager::PropagateGenerators(const TArray<UGridNode*>& Generators)
{
  for (auto& generator : Generators)
  {
    if (!generator->IsConnectedToGrid())
    {
      generator->PropagateNetwork(nullptr);
    }
  }
}

void AGridManager::RemoveNodeFromGrid(FGrid* Grid, UGridNode* Node, float Power)
{
  RemoveNodeFromGrid(Grid->GridID, Node, Power);
}


void AGridManager::LoadManager(bool bRewindLoad)
{
  //The case of ordinary load is taken care at GridNodeGenerator's BeginPlay
  if (bRewindLoad) 
  {
    TArray<UGridNode*> generators;
    for (auto& grid : Grids)
    {
      generators.Append(grid.GetGenerators());
      grid.Reset();
    }
    Grids.Reset();
    PropagateGenerators(generators);
  }
}
