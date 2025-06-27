#include "BlockSMC.h"
#include "../Manager/ManagerSubsystem.h"
#include "BlockActor.h"
#include "BlockLibrary.h"

UBlockSMC::UBlockSMC()
{
  DefaultMaterial = GetMaterial(0);
  WorldPositionOffsetDisableDistance = 5000.f;
}

void UBlockSMC::AddNewConnection(FName hostSocket, FSocketData ForeignSocket, UPhysicsConstraintComponent* constraint)
{
  SocketMap.Add(hostSocket, ForeignSocket);
  ConstraintMap.Add(hostSocket, constraint);
}

void UBlockSMC::RemoveConnection(FName socketName)
{
  SocketMap.Remove(socketName);
  ConstraintMap.Remove(socketName);
}

void UBlockSMC::DetachComponent()
{
  for (auto& constraint : ConstraintMap)
  {
    if (IsValid(constraint.Value))
    {
      // constraint.Value->OnConstraintBroken.Clear();  uncomment this if constraints are breakable
      constraint.Value->Rename();
      constraint.Value->BreakConstraint(); //if(!constraint.Value->IsBroken())
      constraint.Value->DestroyComponent();
    }
  }

  UManagerSubsystem& ManagerSubsystem = UManagerSubsystem::Get(this);

  for (auto& socket : SocketMap)
  {
    ManagerSubsystem.BlockManager->RemoveConnectionSaveData(FSocketData(this, socket.Key), socket.Value);
    socket.Value.Primitive->RemoveConnection(socket.Value.Socket);
  }

  ConstraintMap.Empty();
  SocketMap.Empty();
}

void UBlockSMC::OnConstraintBroken(int32 ConstraintIndex)
{
  DetachComponent();
}

void UBlockSMC::BreakConnection(UBlockSMC* other)
{

  UManagerSubsystem& ManagerSubsystem = UManagerSubsystem::Get(this);

  for (auto& socket : SocketMap)
  {
    if (socket.Value.Primitive == other)
    {
      FName SocketToBreak = socket.Key;
      UPhysicsConstraintComponent* constraint = *ConstraintMap.Find(SocketToBreak);
      ManagerSubsystem.BlockManager->RemoveConnectionSaveData(FSocketData(this, SocketToBreak), socket.Value);

      constraint->BreakConstraint();
      constraint->DestroyComponent();
      RemoveConnection(SocketToBreak);
      socket.Value.Primitive->RemoveConnection(socket.Value.Socket);

      break;
    }
  }
}

TMap<FName, FSocketData> UBlockSMC::GetSocketMap() const
{
  return SocketMap;
}

bool UBlockSMC::IsSocketOfType(FName SocketName, EBlockSocketType Type) const
{
  FString SocketType = UEnum::GetValueAsString(Type);
  SocketType.ReplaceInline(TEXT("EBlockSocketType::"), TEXT(""));

  return SocketName.ToString().Contains(SocketType);
}

void UBlockSMC::Detach(TArray<UBlockSMC*>& DetachedComps)
{
  TArray<UBlockSMC*> comps;

 // DetachedComps = GetAttachedComponents();
//  if (DetachedComps.Num() > 0)
 // {
    ABlockActor* BlockActor = Cast<ABlockActor>(GetOwner());
    BlockActor->GetComponents<UBlockSMC>(comps); // if (this->GetOwner()->Implements<UItemDataInterface>())
    BlockActor->OnDetached.Broadcast();

    for (UBlockSMC* comp : comps)
    {
      DetachedComps.Append(comp->GetAttachedComponents());
      comp->DetachComponent();
    }
 // }
}

EBlockType UBlockSMC::GetBlockType()
{
  return Cast<ABlockActor>(GetOwner())->GetBlockType();
}
 
bool UBlockSMC::IsNetworkStatic()
{
  for (UBlockSMC* comp : GetNetworkOfAttachedComponents(this))
  {
    if (comp->GetBlockType() == EBlockType::Static) return true;
  }
  return false;
}

bool UBlockSMC::IsNetworkSemiStatic()
{
  for (UBlockSMC* comp : GetNetworkOfAttachedComponents(this))
  {
    if (comp->GetBlockType() != EBlockType::Dynamic) return true;
  }
  return false;
}

void UBlockSMC::DisableSimWhenSleep()
{
  BodyInstance.bGenerateWakeEvents = true;
  OnComponentSleep.AddUniqueDynamic(this, &UBlockSMC::DisableSim);
}

void UBlockSMC::DisableSim()
{
  SetSimulatePhysics(false);
  BodyInstance.bGenerateWakeEvents = false;
}

void UBlockSMC::DisableSim(UPrimitiveComponent* comp, FName bone)
{
  DisableSim();
}

void UBlockSMC::EnableSim()
{
  SetSimulatePhysics(true);
  OnComponentSleep.Clear();
  BodyInstance.bGenerateWakeEvents = true;
}


TArray<FSocketData> UBlockSMC::GetFreeSockets(EBlockSocketType Type)
{
  TArray<FSocketData> tempSockets;
  for (FName socketname : this->GetAllSocketNames())
  {
    if (!SocketMap.Contains(socketname) && IsSocketOfType(socketname, Type))
    {
      tempSockets.Add(FSocketData(this, socketname));
    }
  }
  return tempSockets;
}


TArray<FSocketData> UBlockSMC::GetActorFreeSockets(EBlockSocketType Type) const
{ 
  TArray<UBlockSMC*> comps;
  TArray<FSocketData> freeSockets;

  GetOwner()->GetComponents<UBlockSMC>(comps);

  for (UBlockSMC* comp : comps)
  {
    freeSockets.Append(comp->GetFreeSockets(Type));
  }
  return freeSockets;
}

TArray<UBlockSMC*> UBlockSMC::GetAttachedComponents()
{
  TArray<UBlockSMC*> AttachedComps;
  for (auto& socket : SocketMap)
  {
    AttachedComps.AddUnique(socket.Value.Primitive);
  }
  return AttachedComps;
}

TArray<UBlockSMC*> UBlockSMC::GetNetworkOfAttachedComponents(UBlockSMC* CompBefore)
{
  TArray<UBlockSMC*> AttachedComps;
  AttachedComps.AddUnique(this);

  for (UBlockSMC* comp : GetAttachedComponents())
  {
    if (comp != CompBefore)
    {
      AttachedComps.Append(comp->GetNetworkOfAttachedComponents(this));
    }
  }

  return AttachedComps;
}


bool UBlockSMC::IsFree()
{
  return SocketMap.IsEmpty();
}

bool UBlockSMC::AreSocketsAttached(FName socket, FSocketData& _other)
{
  if (FSocketData* other = SocketMap.Find(socket)) //Use FindRef() instead of Find(). Never deref a custom struct. It will crash the packaged build
  {
    return *other == _other; 
  }
  return false;
}


TArray<AActor*> UBlockSMC::FindAttachedActors(AActor* actorBefore)
{
  //This function assumes these are all Block actors

  TArray<AActor*> attachedActors;
  TArray<UBlockSMC*> comps;

  GetOwner()->GetComponents<UBlockSMC>(comps);

  attachedActors.AddUnique(GetOwner());

  for (UBlockSMC* comp : comps)
  {
    for (auto& socket : comp->SocketMap)
    {
      if (socket.Value.Primitive->GetOwner() != actorBefore)
      {
        attachedActors.Append(socket.Value.Primitive->FindAttachedActors(GetOwner()));
      }      
    }
  }
  return attachedActors;
}

void UBlockSMC::SetWireframe(const bool wireframe)
{
  bIsWireframe = wireframe;
}

bool UBlockSMC::IsWireframe() const
{
  return bIsWireframe;
}

void UBlockSMC::ResetMaterialToDefault()
{
  SetMaterial(0, DefaultMaterial);
}


FName UBlockSMC::GetItemKey_Implementation() const
{
 // IItemDataInterface* actor = Cast<IItemDataInterface>(GetOwner());
 // return actor->Execute_GetItemKey();
 return IItemDataInterface::Execute_GetItemKey(GetOwner());

}

bool UBlockSMC::IsItemActor_Implementation() const
{
  return IItemDataInterface::Execute_IsItemActor(GetOwner());
}

void UBlockSMC::TransformFuseObject_Implementation(FName FusedItem, int32 Amount)
{
  return IFuseInterface::Execute_TransformFuseObject(GetOwner(), FusedItem, Amount);
}

void UBlockSMC::GrabObject_Implementation(bool bIsGrabbed)
{
  Cast<ABlockActor>(GetOwner())->bIsGrabbed = bIsGrabbed;
}

bool UBlockSMC::IsObjectGrabbed_Implementation() const
{
  return Cast<ABlockActor>(GetOwner())->bIsGrabbed;
}

void UBlockSMC::OnActorReloaded_Implementation(bool RewindLoad)
{
  if (RewindLoad)
  {
    DetachComponent();
  }

}

/*
void UBlockSMC::OnActorLoaded_Implementation()
{
  
  for (auto& elem : SocketMap)
  {
    UBlockLibrary::ConnectSockets(FSocketData(this, elem.Key), elem.Value);
  } 
  
}
*/