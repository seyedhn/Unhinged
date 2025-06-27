#include "BlockActor.h"
#include "BlockSMC.h"
#include "../Rewind/RewindComp.h"
#include "ItemDataSubsystem.h"
#include "../Spline/PipeInterface.h"
#include "../ItemsData/ItemTags.h"
#include "../ItemsData/ItemPropertyStructs.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../Damage/DamageLibrary.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "../Messaging/PayloadStructs.h"
#include "../Messaging/ChannelTags.h"
#include "../Manager/ManagerSettings.h"


ABlockActor::ABlockActor()
{
}

void ABlockActor::BeginPlay()
{
  Super::BeginPlay();
  if(!bIsSpawnLoaded) SetBlockData();
//  if (bIsMaterialised) Materialize();

}

void ABlockActor::PreInitializeComponents()
{
  Super::PreInitializeComponents();

  GetComponents<UBlockSMC>(Comps);
  if (UBlockSMC* root = Cast<UBlockSMC>(RootComponent))
  {
    RootComp = root;
    return;
  }
  if (Comps.Num() > 0)
  {
    RootComp = Comps[0];
  }
}

void ABlockActor::OnActorReloaded_Implementation(bool RewindLoad)
{
  if (!RewindLoad) SetBlockData();
  SetChargedMaterial();
}

void ABlockActor::OnActorSpawnLoaded_Implementation(bool RewindLoad)
{
  SetBlockData();
  SetChargedMaterial();
}

FName ABlockActor::GetItemKey_Implementation() const
{
  return Key;
}

bool ABlockActor::IsItemActor_Implementation() const
{
  return true;
}

bool ABlockActor::IsCharged_Implementation(URewindComp*& RewindComp) const
{
  RewindComp = GetRewindComp();
  if (RewindComp) return true;
  return false;

}

void ABlockActor::BecomeCharged_Implementation(bool Charged, UPrimitiveComponent* HitComp)
{
  if (Charged)
  {

    for (UBlockSMC* comp : Comps)
    {
      comp->SetMaterial(0, GetDefault<UManagerSettings>()->BlockChargedMaterial.LoadSynchronous());
      //comp->SetCustomPrimitiveDataFloat(0, 1.f);
    }
    URewindComp* RewindComp = Cast<URewindComp>(AddComponentByClass(URewindComp::StaticClass(), false, FTransform(), true));
    RewindComp->RewindOwner = this;
    RewindComp->RewindableComp = RootComp;
    FinishAddComponent(RewindComp, false, FTransform());
    return;
  }

  for (UBlockSMC* comp : Comps)
  {
    comp->ResetMaterialToDefault();
  }
  if (URewindComp* RewindComp = GetComponentByClass<URewindComp>())
  {
    RewindComp->Rename();
    RewindComp->DestroyComponent();
  }
}

bool ABlockActor::IsRewindable_Implementation() const
{
  return (BlockType == EBlockType::Dynamic);
}


void ABlockActor::SetBlockData()
{
  UItemDataSubsystem& ItemDataSubsystem = UItemDataSubsystem::Get(this);
  TArray<UItemDataAsset*> DAs = ItemDataSubsystem.FindItemsOfClass(this->GetClass());
  if (DAs.Num() > 0)
  {
    Key = DAs[0]->GetItemKey(); //Assuming there is only one DA for each Block Actor.
    SetMass(DAs[0]);

    if (ItemDataSubsystem.ItemHasTag(Key, FItemTags::Get().BlockDynamic)) BlockType = EBlockType::Dynamic;
    else if (ItemDataSubsystem.GetItemTags(Key).HasTag(FItemTags::Get().BlockStatic)) BlockType = EBlockType::Static;
    else if (ItemDataSubsystem.GetItemTags(Key).HasTag(FItemTags::Get().StructureStatic)) BlockType = EBlockType::Static;
    else if (ItemDataSubsystem.GetItemTags(Key).HasTag(FItemTags::Get().StructureSpline)) BlockType = EBlockType::Spline;
    else BlockType = EBlockType::Dynamic;

    if (BlockType == EBlockType::Dynamic)
    {
      for (auto& comp : Comps)
      {
        comp->SetNotifyRigidBodyCollision(true);
        comp->OnComponentHit.AddUniqueDynamic(this, &ABlockActor::OnHit);
      }
    }   
  }
}

void ABlockActor::SetBlockType()
{
}

void ABlockActor::SetMass(UItemDataAsset* DA)
{
  int numcomps = Comps.Num();

  bool HasPhysProp;
  FInstancedStruct property = DA->GetItemProperty(FItemPhysicalProperties::StaticStruct(), HasPhysProp);
  if (HasPhysProp)
  {
    Mass = property.Get<FItemPhysicalProperties>().Mass;

    for (UBlockSMC* comp : Comps)
    {
      comp->SetMassOverrideInKg(NAME_None, Mass / numcomps, true);
    }
  }

}

bool ABlockActor::IsNetworkStatic(ABlockActor* actoreBefore)
{
  for (ABlockActor* block : GetNetworkOfAttachedBlocks(actoreBefore, EBlockSocketType::Attach))
  {
    if (block->GetBlockType() == EBlockType::Static) return true;
  }
  return false;
}

EBlockType ABlockActor::GetBlockType() const
{
  return BlockType;
}

TArray<ABlockActor*> ABlockActor::GetNetworkOfAttachedBlocks(ABlockActor* actoreBefore, EBlockSocketType Type)
{
 
  TArray<ABlockActor*> attachedActors; 
  attachedActors.AddUnique(this);
  
  for (UBlockSMC* comp : Comps)
  {
    for (auto& socket : comp->GetSocketMap())
    {
      if (socket.Value.IsSocketValid())
      {
        ABlockActor* SocketOwner = StaticCast<ABlockActor*>(socket.Value.Primitive->GetOwner());
        if (SocketOwner != actoreBefore && socket.Value.IsSocketOfType(Type))
        {
          attachedActors.Append(SocketOwner->GetNetworkOfAttachedBlocks(this, Type));
        }
      }

    }
  }
  return attachedActors;
}

void ABlockActor::GetAttachedBlocks(TSet<ABlockActor*> IgnoreBlocks, EBlockSocketType Type, TArray<ABlockActor*>& AttachedBlocks, TArray<FSocketData>& AttachedSockets, TArray<UBlockSMC*>& HostComps)
{

  for (UBlockSMC* comp : Comps)
  {
    for (auto& socket : comp->GetSocketMap())
    {
      if (socket.Value.IsSocketValid())
      {
        ABlockActor* SocketOwner = StaticCast<ABlockActor*>(socket.Value.Primitive->GetOwner());
        if (!IgnoreBlocks.Contains(SocketOwner) && socket.Value.IsSocketOfType(Type))
        {
          AttachedBlocks.AddUnique(SocketOwner);
          HostComps.Add(comp);
          AttachedSockets.Add(socket.Value);
        }
      }

    }
  }

}

TArray<FSocketData> ABlockActor::GetFreeSockets(EBlockSocketType Type) const
{
  return RootComp->GetActorFreeSockets(Type);
}

void ABlockActor::FlowObjectToNextBlock(ABlockActor* PreviousBlock, FName Object, EBlockSocketType SocketType)
{
  TSet<ABlockActor*> IgnoreBlocks = TSet<ABlockActor*>{PreviousBlock};
  TArray<ABlockActor*> AttachedBlocks;
  TArray<FSocketData> AttachedSockets;
  TArray<UBlockSMC*> HostComps;
  GetAttachedBlocks(IgnoreBlocks, SocketType, AttachedBlocks, AttachedSockets, HostComps);

  if (AttachedBlocks.Num() == 0)
  {
    IPipeInterface::Execute_SelfFlow(this, Object);
    return;
  }

  for (uint8 i = 0; i < AttachedBlocks.Num(); i++)
  {
    if (AttachedBlocks[i]->Implements<UPipeInterface>())
    {
      IPipeInterface::Execute_ObjectFlows(AttachedBlocks[i], Object, this, AttachedSockets[i]);
      return;
    }  
  }
  IPipeInterface::Execute_SelfFlow(this, Object); //For blocks that have more than 2 sockets (such as splitters), need to call SelfFlow on that specific socket. This would need updating later.
}

void ABlockActor::DestroyComponents(TArray<UActorComponent*> components)
{
  for (auto& comp : components)
  {
    if (comp)
    {
      comp->DestroyComponent();
    }    
  }
}

void ABlockActor::EnableSim(bool Enable)
{
  if (Enable)
  {
    RootComp->EnableSim();
    return;
  }
  RootComp->DisableSim();
}

void ABlockActor::SetBlockTransform(FTransform transform)
{
  RootComp->SetWorldTransform(transform, false, nullptr, ETeleportType::TeleportPhysics);
  
  //COME BACK TO THIS LATER
  //Need to keep the attached blocks awake, otherwise they get left behind when going to sleep. This is highly unoptimised to search for network at every frame.
  /*
  for (auto& comp : RootComp->GetNetworkOfAttachedComponents(RootComp))
  {
    comp->WakeRigidBody();
  }*/
}

FTransform ABlockActor::GetBlockTransform()
{
  return RootComp->GetComponentTransform();
}

float ABlockActor::GetBlockVelSq()
{
  return RootComp->GetComponentVelocity().SquaredLength();
}

void ABlockActor::SetChargedMaterial()
{
  UMaterialInterface* material;
  if (GetRewindComp())
  {
    material = GetDefault<UManagerSettings>()->BlockChargedMaterial.LoadSynchronous();
    for (UBlockSMC* comp : Comps)
    {
      comp->SetMaterial(0, material);
    }
  }
  else
  {
    for (UBlockSMC* comp : Comps)
    {
      comp->ResetMaterialToDefault();
    }
  }
}


void ABlockActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
  UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
  MessageSubsystem.BroadcastMessage(FChannelTags::Get().Collision, FPayloadCollision(HitComponent, OtherComp, NormalImpulse, Hit.Location, Hit.Normal, Hit.PhysMaterial));

  if (!bIsGrabbed)
  {
    TryDealDamage(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
  }

}


void ABlockActor::TryDealDamage(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
  float Vel = HitComponent->GetComponentVelocity().Length();
  if (Vel > 500)
  {
    TemporarilyDisableHitEvent(HitComponent);
    UItemDataSubsystem& ItemDataSubsystem = UItemDataSubsystem::Get(this);
    float Damage = Mass*Vel*0.02;
    EDamageType DamageType = EDamageType::Smash;

    if (ItemDataSubsystem.ItemHasProperty<FItemDamager>(Key))
    {
      FItemDamager DamageProps = UItemDataSubsystem::Get(this).GetItemProperty<FItemDamager>(Key);
      DamageType = DamageProps.DamageType;
      Damage *= DamageProps.Damage;
    }

    UDamageLibrary::DealHitDamage(this, OtherActor, OtherComp, Damage, DamageType, Hit.Item, Hit.Location, Hit.Normal, Hit.BoneName);

  }
}

void ABlockActor::TemporarilyDisableHitEvent(UPrimitiveComponent* HitComponent)
{
  FTimerHandle GenerateHitEventTimer;
  FTimerDelegate GenerateHitEventDelegate;
  HitComponent->SetNotifyRigidBodyCollision(false);
  GenerateHitEventDelegate.BindLambda([HitComponent] {HitComponent->SetNotifyRigidBodyCollision(true); });
  GetWorld()->GetTimerManager().SetTimer(GenerateHitEventTimer, GenerateHitEventDelegate, 1.f, false);
}



URewindComp* ABlockActor::GetRewindComp() const
{
  return GetComponentByClass<URewindComp>();
}

void ABlockActor::Materialize_Implementation()
{
  bIsMaterialised = true;
  OnMaterialise.Broadcast();
}

void ABlockActor::SetWireframe(const bool wireframe)
{
  for (UBlockSMC* comp : Comps)
  {
    comp->SetWireframe(wireframe);
  }
}

void ABlockActor::DetachBlock()
{
  TArray<UBlockSMC*> DetachedComps;
  for (UBlockSMC* comp : Comps)
  {
    comp->Detach(DetachedComps);
  }
}

void ABlockActor::DestroyBlock()
{
  DetachBlock();
  Destroy();
}

void ABlockActor::BindToEquipment(FGameplayTag Channel)
{
  MessageListenerHandle = UGameplayMessageSubsystem::Get(this).RegisterListener(FChannelTags::Get().Sleep, this, &ThisClass::OnMessageReceivedFromEquipment);
}

void ABlockActor::OnMessageReceivedFromEquipment_Implementation(FGameplayTag Channel, const FInstancedStruct& Payload)
{
  MessageListenerHandle.Unregister();
}



/*
  for (UBlockSMC* comp : Comps)
  {
   // UKismetMathLibrary::NormalizedDeltaRotator(comp->GetRelativeRotation(), Comps[0]->GetRelativeRotation())
    //Come back to this later. This is not the correct way of calculating relative transform. See GrabAttach_FL -> GetLerpTargetTransformFromChildComp.
    CompLocalTransforms.Add(FTransform(comp->GetRelativeRotation() - Comps[0]->GetRelativeRotation(),
                            comp->GetRelativeLocation() - Comps[0]->GetRelativeLocation(),
                            comp->GetRelativeScale3D()));
  }

    
  for (int32 i =0; i < Comps.Num(); ++i)
  {
    //Come back to this later. This is not the correct way of calculating relative transform. See GrabAttach_FL -> GetLerpTargetTransformFromChildComp.
    Comps[i]->SetWorldTransform(FTransform(UKismetMathLibrary::ComposeRotators(transform.Rotator(), CompLocalTransforms[i].Rotator()),
                                transform.GetLocation() + CompLocalTransforms[i].GetLocation(),
                                CompLocalTransforms[i].GetScale3D()), false, nullptr, ETeleportType::TeleportPhysics);
  }

    
  if (Enable)
  {
    for (UBlockSMC* comp : Comps)
    {
      comp->EnableSim();
    }
    return;
  }
  for (UBlockSMC* comp : Comps)
  {
     comp->DisableSim();
    //if (comp != Comps[0])  comp->AttachToComponent(Comps[0], FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));
  }
*/