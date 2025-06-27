#include "PipeNetwork.h"
#include "../ItemsData/ItemPropertyStructs.h"
#include "../Save/SaveGameInterface.h"
#include "../Save/SaveGameSubsystem.h"


void FPipeNetwork::AddBlock(ABlockActor* block)
{
  UItemDataSubsystem& ItemDataSubsystem = UItemDataSubsystem::Get(block);
  if (ItemDataSubsystem.ItemHasProperty<FItemPipeData>(block))
  {
    EPipeType Type = ItemDataSubsystem.GetItemProperty<FItemPipeData>(block).PipeType;

    switch (Type)
    {
    case EPipeType::Consumer:
      ConsumerBlocks.Add(block);
      break;
    case EPipeType::Source:
      SourceBlocks.Add(block);
      break;
    }
  }

}


bool FPipeNetwork::NetworkContainsSource(ABlockActor* source) const
{
	return SourceBlocks.Contains(source);
}

bool FPipeNetwork::IsNetworkValid() const
{
  return SourceBlocks.Num() > 0 && ConsumerBlocks.Num() > 0;
}

TArray<ABlockActor*> FPipeNetwork::GetSourceBlocks() const
{
  return SourceBlocks.Array();
}


void FPipeNetwork::SetBlockIDs()
{
	TSet<ABlockActor*> blocks = SourceBlocks.Union(ConsumerBlocks);
	for (auto& block : blocks)
	{
		BlockIDs.Add(ISaveGameInterface::Execute_GetActorID(block));
	}
}


void FPipeNetwork::SetBlocksFromIDs(UObject* WorldContextObject)
{
  USaveGameSubsystem& SaveGameSubsystem = USaveGameSubsystem::Get(WorldContextObject);
  for (auto& ID : BlockIDs)
  {
     AddBlock(static_cast<ABlockActor*>(SaveGameSubsystem.GetActorFromID(ID)));
  }
}