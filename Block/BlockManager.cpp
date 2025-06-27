#include "BlockManager.h"
#include "BlockActor.h"
#include "SocketConstraint.h"
#include "../Messaging/PayloadStructs.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "ItemDataSubsystem.h"
#include "../Save/SaveGameSubsystem.h"
#include "EngineUtils.h"

ABlockManager::ABlockManager()
{
}

TArray<FName> ABlockManager::GetUnlockedBlocks(const FGameplayTagContainer& TagContainer) const
{
  TArray<FName> blocks = UnlockedBlocks.Array();
  TArray<FName> output;
  for (FName block : blocks)
  {
    FGameplayTagContainer tags = UItemDataSubsystem::Get(this).GetItemTags(block);
    if (tags.HasAny(TagContainer))
    {
      output.Add(block);
    }
  }
  return output;
  //return UnlockedBlocks;
}

void ABlockManager::UnlockBlock(const FName Block)
{
  UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this); 

  MessageSubsystem.BroadcastMessage(FChannelTags::Get().Notification, FPayloadNotification(Block, FText::FromString("Script Installed")));

  UnlockedBlocks.Add(Block);
}

void ABlockManager::AddConnectionSaveData(FSocketData socket1, FSocketData socket2)
{
  ConnectedSockets.Add(FSocketPairData(socket1, socket2));
}

void ABlockManager::RemoveConnectionSaveData(FSocketData socket1, FSocketData socket2)
{
  ConnectedSockets.Remove(FSocketPairData(socket1, socket2));
}

//This function is called on two occasions. Everytime a new connection is made (see BlockLibrary), and when loading up BlockManager to connect all loaded blocks (see LoadManager).
void ABlockManager::ConnectSockets(FSocketData socket1, FSocketData socket2)
{  
  if(socket1.IsSocketValid() && socket2.IsSocketValid())
  {
    USocketConstraint* Constraint = Cast<USocketConstraint>(socket1.Primitive->GetOwner()->AddComponentByClass(USocketConstraint::StaticClass(), true, socket1.Primitive->GetSocketTransform(socket1.Socket, RTS_Component), false));
    Constraint->Rename();
    Constraint->AttachToComponent(socket1.Primitive, FAttachmentTransformRules::KeepRelativeTransform);
    Constraint->SetConstrainedComponents(socket1.Primitive, NAME_None, socket2.Primitive, NAME_None);
    socket1.Primitive->AddNewConnection(socket1.Socket, socket2, Constraint);
    socket2.Primitive->AddNewConnection(socket2.Socket, socket1, Constraint);
    Constraint->OnConstraintBroken.AddDynamic(socket1.Primitive, &UBlockSMC::OnConstraintBroken); //this line causes crashes in packaged build, cuz as soon as constraint is made, it breaks and is destroyed, crashing it.
  }
}

void ABlockManager::LoadManager(bool bRewindLoad)
{
  USaveGameSubsystem& SaveGameSubsystem = *UWorld::GetSubsystem<USaveGameSubsystem>(GetWorld());

  for (FSocketPairData& Pair : ConnectedSockets)
  {
    if (Pair.Socket1.ActorID.IsValid() && Pair.Socket2.ActorID.IsValid())
    {
      AActor* Actor1 = SaveGameSubsystem.GetActorFromID(Pair.Socket1.ActorID);
      AActor* Actor2 = SaveGameSubsystem.GetActorFromID(Pair.Socket2.ActorID);
      UBlockSMC* comp1 = Cast<UBlockSMC>(Actor1->GetDefaultSubobjectByName(Pair.Socket1.ComponentName));
      UBlockSMC* comp2 = Cast<UBlockSMC>(Actor2->GetDefaultSubobjectByName(Pair.Socket2.ComponentName));

      FSocketData Socket1 = FSocketData(comp1, Pair.Socket1.Socket);
      FSocketData Socket2 = FSocketData(comp2, Pair.Socket2.Socket);

      //This is not too optimised in the case of RewindLoad, because all constraints are destroyed (see OnActorLoaded implementation of BlockSMC)
      // and created again in the ConnectSockets function.  
      //Note that the SocketCOnstraint type is never spawned in the SaveGameSubsystem. It has been disallowed by its type, as a safety measure that
      //all constraints are spawned fresh based on ConnectedSocket data that we have.
      ConnectSockets(Socket1, Socket2);
   
    }
  }
}

