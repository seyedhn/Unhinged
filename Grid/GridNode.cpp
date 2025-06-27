#include "GridNode.h"
#include "GridInterface.h"
#include "../Manager/ManagerSubsystem.h"
#include "../Manager/ManagerSettings.h"
#include "../Grid/GridManager.h"
#include "../Block/BlockActor.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UGridNode::UGridNode()
{
  bIsPowered = true;
  Power = IdlePower;
}

void UGridNode::BeginPlay()
{
  Super::BeginPlay();

  if (ABlockActor* Owner = Cast<ABlockActor>(GetOwner()))
  {
    Owner->OnDestroyed.AddDynamic(this, &UGridNode::OnDestroyed);
    Owner->OnMaterialise.AddDynamic(this, &UGridNode::OnMaterialize);
  }
}


//ADD or REMOVE neighbours

void UGridNode::AddNeighbour(UGridNode* Other)
{
  UNiagaraComponent* Tether = MakeTether(Other);
  Connections.Add(FGridConnection(Other, Tether));
  Other->Connections.Add(FGridConnection(this, Tether));
}

UNiagaraComponent* UGridNode::MakeTether(UGridNode* Other)
{
  UNiagaraSystem* Emitter = GetDefault<UManagerSettings>()->Tether.LoadSynchronous();
  FVector location = GetSocketLocation(TEXT("Grid"));
  FVector distance = Other->GetSocketLocation(TEXT("Grid")) - location;
  FRotator rotation = UKismetMathLibrary::MakeRotFromX(distance);
  UNiagaraComponent* Tether = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, Emitter, location, rotation, FVector(1.f), false, true, ENCPoolMethod::None, false);
  Tether->SetVectorParameter(TEXT("Beam End"), FVector(distance.Length(), 0.f, 0.f));
  return Tether;
}




void UGridNode::RemoveNeighbour(UGridNode* Other)
{
  int32 index = Connections.IndexOfByKey(Other); //For safety, we may need to check that the return is not INDEX_NONE
  Connections[index].DestroyTether();
  Connections.RemoveAtSwap(index);  //We only do it one way, because Other is being destroyed anyways
}


//SEARCH//

TArray<UGridNode*> UGridNode::FindNearbyNodes()
{
  TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType{ UEngineTypes::ConvertToObjectType(GetCollisionObjectType()) };
  TArray<AActor*> ActorsToIgnore{ GetOwner() };
  TArray<UPrimitiveComponent*> Comps;

  UKismetSystemLibrary::SphereOverlapComponents(this, GetComponentLocation(), RangeRadius, ObjectType, UGridNode::StaticClass(), ActorsToIgnore, Comps);
  TArray<UGridNode*>& Grids = reinterpret_cast<TArray<UGridNode*>&>(Comps);

  SortNodesByDistance(Grids);

  return Grids;
}

void UGridNode::SortNodesByDistance(TArray<UGridNode*>& Nodes)
{
  TSortedMap<float, UGridNode*> SortMap;
  for (auto& node : Nodes)
  {
    SortMap.Add((node->GetComponentLocation() - GetComponentLocation()).SquaredLength(), node);
  }
  SortMap.GenerateValueArray(Nodes);
}


UGridNode* UGridNode::FindNearestNode()
{
  TArray<UGridNode*> nodes = FindNearbyNodes();
  if (nodes.Num() > 0)
  {
    return nodes[0];
  }
  return nullptr;
}

UGridNode* UGridNode::FindNearestFreeNode()
{
  TArray<UGridNode*> nodes = FindNearbyNodes();
  if (nodes.Num() > 0)
  {
    for (auto& node : nodes)
    {
      if (node->CanTakeConnection()) return node;
    }
  }
  return nullptr;
}

//GETTERS - SETTERS

void UGridNode::Reset()
{
  for (auto& connection : Connections)
  {
    connection.DestroyTether();
  }
  Connections.Reset();
  GridID.Invalidate();
  bIsPowered = false;
}


void UGridNode::OnPowerRestore()
{
  bIsPowered = true;
  UpdateTetherColour(true);
  PlayPowerSound(true);
  if (GetOwner()->Implements<UGridInterface>())
    IGridInterface::Execute_OnPowerRestore(GetOwner());
}

void UGridNode::OnPowerOutage()
{
  bIsPowered = false;
  UpdateTetherColour(false);
  PlayPowerSound(false);
  if(GetOwner()->Implements<UGridInterface>())
    IGridInterface::Execute_OnPowerOutage(GetOwner());
}

void UGridNode::PlayPowerSound(bool Restore)
{
  USoundBase* Sound = Restore ? RestoreSound : OutageSound;
 // if(!PowerSound->IsValidLowLevel() || !PowerSound->GetOwner() || !PowerSound->GetOwner()->IsValidLowLevel())
//  {
    PowerSound = UGameplayStatics::SpawnSoundAtLocation(GetOwner(), Sound, GetComponentLocation(), UE::Math::TRotator<double>::ZeroRotator, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    return;
 // }
//  PowerSound->SetSound(Sound); This make game crash with level streaming on when player recalls
//  PowerSound->Play();
}

void UGridNode::UpdateTetherColour(bool bActive)
{
  FLinearColor color = bActive ? FLinearColor(0.f, 5.f, 100.f) : FLinearColor(100.f, 5.f, 0.f);
  for (auto& connection : Connections)
  {
    connection.Tether->SetColorParameter(TEXT("Color"), color);
  }
}


bool UGridNode::IsConnectedToGrid() const
{
  return GridID.IsValid();
}

bool UGridNode::IsPowered() const
{
  return IsConnectedToGrid() && bIsPowered;
}


UGridNode* UGridNode::GetNeighbour() const
{
  return Connections[0].Node;
}

FGuid UGridNode::GetGridID() const
{
  return GridID;
}

