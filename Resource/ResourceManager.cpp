#include "ResourceManager.h"
#include "Kismet/GameplayStatics.h"
#include "../Manager/ManagerSubsystem.h"

AResourceManager::AResourceManager()
{
  CentralStorage = CreateDefaultSubobject<UStorageComp>(FName(TEXT("CentralStorage")));
  CentralStorage->SetStorageCapacity(24);
}

void AResourceManager::BeginPlay()
{
  Super::BeginPlay();
  SetBackpackData();
  SetResourceContainer();
}

void AResourceManager::SetResourceContainer()
{
  ResourceContainer = Cast<AResourceContainer>(UGameplayStatics::GetActorOfClass(this, AResourceContainer::StaticClass()));
  if (!IsValid(ResourceContainer)) ResourceContainer = GetWorld()->SpawnActor<AResourceContainer>(AResourceContainer::StaticClass(), FActorSpawnParameters());
}

void AResourceManager::SetBackpackData()
{
  Backpack = UManagerSubsystem::Get(this).Avatar->Backpack;
}