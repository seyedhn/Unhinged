#include "ResourceContainer.h"
#include "UObject/ConstructorHelpers.h"
#include "../Manager/ManagerSettings.h"


AResourceContainer::AResourceContainer()
{
  USceneComponent* DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName(TEXT("DefaultSceneRoot")));
  RootComponent = DefaultSceneRoot;
}

void AResourceContainer::BeginPlay()
{
  Super::BeginPlay();
  
  FString bvalid = IsValid(this) ? "true" : "false";
  int32 aaa = GetComponents().Num();
  UE_LOG(LogTemp, Warning, TEXT("%i"), aaa);
}

void AResourceContainer::SpawnItem(const FName Key, const int32 Amount, const FVector Location, const FVector Impulse)
{
  UClass* ResourceSMC_Class = GetDefault<UManagerSettings>()->ResourceSMC.LoadSynchronous();
  UActorComponent* SpawnedComp = AddComponentByClass(ResourceSMC_Class, true, FTransform(), false);
  UResourceSMC* spawnedResource = Cast<UResourceSMC>(SpawnedComp);
  spawnedResource->SpawnItself(Key, Amount, Location, Impulse);
}
