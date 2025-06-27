#include "PipeManager.h"
#include "PipeInterface.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "../Messaging/ChannelTags.h"
#include "../Block/BlockSocket.h"
#include "ItemDataSubsystem.h"


void APipeManager::BeginPlay()
{
  Super::BeginPlay();
  //this is being binded in both BeginPlay and LoadManager. Need to avoid this.
  UGameplayMessageSubsystem::Get(this).RegisterListener(FChannelTags::Get().PipeNetwork, this, & APipeManager::TryEstablishNetwork);
  UGameplayMessageSubsystem::Get(this).RegisterListener(FChannelTags::Get().PipeStartFlow, this, &APipeManager::RunFluidInNetwork);
}


void APipeManager::TryEstablishNetwork(FGameplayTag Channel, const FPayloadKeyBlock& Payload)
{
  FPipeNetwork Network;
  TArray<ABlockActor*> blocks = Payload.Block->GetNetworkOfAttachedBlocks(nullptr, EBlockSocketType::Pipe);

  for (auto block : blocks)
  {
    Network.AddBlock(block);
  }

  if (Network.IsNetworkValid() && !PipeNetworks.Contains(Network))
  {
    EstablishNetwork(Network);
  }
}

void APipeManager::EstablishNetwork(const FPipeNetwork& Network)
{
  PipeNetworks.Add(Network);
  for (auto& source : Network.GetSourceBlocks())
  {
    IPipeInterface::Execute_NetworkEstablished(source, true);
  }
}


void APipeManager::RunFluidInNetwork(FGameplayTag Channel, const FPayloadKeyBlock& Payload)
{
  if (FPipeNetwork* network = PipeNetworks.Find(FPipeNetwork(Payload.Block)))
  {
    for (auto& source : network->GetSourceBlocks())
    {
      source->FlowObjectToNextBlock(nullptr, Payload.Key, EBlockSocketType::Pipe);
    }
  }
}

//the only reason we have this function is so we can use it in BP. Once PipeManager is moved to C++, we can get rid of this function and directly use AddBlock().
void APipeManager::AddBlockToNetwork(ABlockActor* block, FPipeNetwork& Network)
{
  Network.AddBlock(block);
}



void APipeManager::SaveManager()
{
  for (auto& network : PipeNetworks)
  {
    network.SetBlockIDs();
  }
  
}

void APipeManager::LoadManager(bool RewindLoad)
{
  UGameplayMessageSubsystem::Get(this).RegisterListener(FChannelTags::Get().PipeStartFlow, this, &APipeManager::RunFluidInNetwork);


  if (true)//!RewindLoad)
  {
    for (auto& network : PipeNetworks)
    {
      network.SetBlocksFromIDs(this);
    }
  }
}
