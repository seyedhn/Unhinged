#include "ManagerSubsystem.h"
#include "ManagerSettings.h"
#include "../Tools/CheatSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "../Save/SaveGameSubsystem.h"

void UManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void UManagerSubsystem::Deinitialize()
{
}

UManagerSubsystem& UManagerSubsystem::Get(const UObject* WorldContextObject)
{
  UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
  check(World);
  UManagerSubsystem* ManagerSubsystem = UGameInstance::GetSubsystem<UManagerSubsystem>(World->GetGameInstance());
  check(ManagerSubsystem);
  return *ManagerSubsystem;  
}

UManagerSubsystem& UManagerSubsystem::Get(const UWorld* World)
{
  UManagerSubsystem* ManagerSubsystem = UGameInstance::GetSubsystem<UManagerSubsystem>(World->GetGameInstance());
  check(ManagerSubsystem);
  return *ManagerSubsystem;
}

void UManagerSubsystem::InitialiseManagerActors()
{
  DamageManager = GetWorld()->SpawnActor<ADamageManager>(GetDefault<UManagerSettings>()->DamageManager.LoadSynchronous(), FActorSpawnParameters());
  PostprocessManager = GetWorld()->SpawnActor<APostprocessManager>(GetDefault<UManagerSettings>()->PostprocessManager.LoadSynchronous(), FActorSpawnParameters());
  AudioManager = GetWorld()->SpawnActor<AAudioManager>(GetDefault<UManagerSettings>()->AudioManager.LoadSynchronous(), FActorSpawnParameters());
  HeatManager = GetWorld()->SpawnActor<AHeatManager>(GetDefault<UManagerSettings>()->HeatManager.LoadSynchronous(), FActorSpawnParameters());
  NotificationManager = GetWorld()->SpawnActor<ANotificationManager>(GetDefault<UManagerSettings>()->NotificationManager.LoadSynchronous(), FActorSpawnParameters());
  SkyManager = GetWorld()->SpawnActor<ASkyManager>(GetDefault<UManagerSettings>()->SkyManager.LoadSynchronous(), FActorSpawnParameters());
  UserSettingsActor = GetWorld()->SpawnActor<AUserSettingsActor>(GetDefault<UManagerSettings>()->UserSettingsActor.LoadSynchronous(), FActorSpawnParameters());

  CombustionManager = GetWorld()->SpawnActor<ACombustionManager>(ACombustionManager::StaticClass(), FActorSpawnParameters());
  ResourceManager = GetWorld()->SpawnActor<AResourceManager>(AResourceManager::StaticClass(), FActorSpawnParameters());
  PipeManager = GetWorld()->SpawnActor<APipeManager>(APipeManager::StaticClass(), FActorSpawnParameters());
  BlockManager = GetWorld()->SpawnActor<ABlockManager>(ABlockManager::StaticClass(), FActorSpawnParameters());
  UpgradeManager = GetWorld()->SpawnActor<AUpgradeManager>(AUpgradeManager::StaticClass(), FActorSpawnParameters());
  GridManager = GetWorld()->SpawnActor<AGridManager>(AGridManager::StaticClass(), FActorSpawnParameters());
  RewindManager = GetWorld()->SpawnActor<ARewindManager>(ARewindManager::StaticClass(), FActorSpawnParameters());
  TrialsManager = GetWorld()->SpawnActor<ATrialsManager>(ATrialsManager::StaticClass(), FActorSpawnParameters());
 
  //Spawn Manager Actors here
  //...
  //...
  //...

//!USaveGameSubsystem::Get(this).DoesSaveGameExist())

  Avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
  if(Avatar) EquipmentManager = Avatar->GetComponentByClass<UEquipmentHandler>();

  if (SandboxMode)
  {
    UCheatSubsystem::Get(this).EnableSandboxMode();
  }

}










