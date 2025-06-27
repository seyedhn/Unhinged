#include "CheatSubsystem.h"
#include "HAL/IConsoleManager.h"
#include "Kismet/GameplayStatics.h"
#include "../Manager/ManagerSettings.h"
#include "../Manager/ManagerSubsystem.h"
#include "../Save/SaveGameSubsystem.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "../GlobalConstants/GlobalConstantsSubsystem.h"
#include "../Messaging/ChannelTags.h"
#include "../Messaging/PayloadStructs.h"


void UCheatSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
  RegisterConsoleCommands();
}

UCheatSubsystem& UCheatSubsystem::Get(UObject* WorldContextObject)
{
  UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
  check(World);
  UCheatSubsystem* CheatSubsystem = UGameInstance::GetSubsystem<UCheatSubsystem>(World->GetGameInstance());
  check(CheatSubsystem);
  return *CheatSubsystem;
}


void UCheatSubsystem::RegisterConsoleCommands()
{
  
  //if(!IConsoleManager::Get().IsNameRegistered(TEXT("dev.sandbox"))) //checks if the commands are already registered or not
  //{
    IConsoleManager::Get().RegisterConsoleCommand(TEXT("dev.sandbox"), TEXT("Enables sandbox mode"),
      FConsoleCommandDelegate::CreateUObject(this, &UCheatSubsystem::EnableSandboxMode));

    IConsoleManager::Get().RegisterConsoleCommand(TEXT("dev.spectator"), TEXT("Toggle spectator mode"),
      FConsoleCommandDelegate::CreateUObject(this, &UCheatSubsystem::ToggleSpectatorMode));

    IConsoleManager::Get().RegisterConsoleCommand(TEXT("dev.save"), TEXT("Save game"),
      FConsoleCommandDelegate::CreateUObject(this, &UCheatSubsystem::SaveGame));

    IConsoleManager::Get().RegisterConsoleCommand(TEXT("dev.load"), TEXT("Reload game as it the player has been recalled"),
      FConsoleCommandDelegate::CreateUObject(this, &UCheatSubsystem::LoadGame));

    IConsoleManager::Get().RegisterConsoleCommand(TEXT("dev.skip"), TEXT("Skips the capsule introduction of the game"),
      FConsoleCommandDelegate::CreateUObject(this, &UCheatSubsystem::SkipIntro));

    IConsoleManager::Get().RegisterConsoleCommand(TEXT("dev.invincible"), TEXT("Dana becomes invincible to any damage"),
      FConsoleCommandDelegate::CreateUObject(this, &UCheatSubsystem::Invincible));

    IConsoleManager::Get().RegisterConsoleCommand(TEXT("dev.armin"), TEXT("dev.skip + dev.sandbox + dev.invincible + dev.spectator"),
      FConsoleCommandDelegate::CreateUObject(this, &UCheatSubsystem::Armin));

     //Add console commands here
     //...
     //...
     //...
 // }



  static TAutoConsoleVariable<int32> SkyVar(TEXT("dev.sky"), 0, TEXT("Sky Config. 1: PurpleDawn. 2: PurpleDawnForWakeUp. 3: WarnFoggySun. 4: ColdFoggySunset. 5: PrettyMoonlight. 6: ChronoHeavy. 7: Hell. 8: ArminsSpecial"), ECVF_Cheat);
  SkyVar->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &UCheatSubsystem::ApplySkyConfig));

  static TAutoConsoleVariable<float> TimeVar(TEXT("dev.time"), 0.f, TEXT("Sky Time. Range: 0 -> 2400"), ECVF_Cheat);
  TimeVar->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &UCheatSubsystem::ChangeSkyTime));

  static TAutoConsoleVariable<float> DayLengthVar(TEXT("dev.daylength"), 0.f, TEXT("Time of day and night"), ECVF_Cheat);
  DayLengthVar->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &UCheatSubsystem::ChangeDayLength));

  static TAutoConsoleVariable<float> FlightLiftVar(TEXT("dev.lift"), 0.f, TEXT("The lift strength factor of the wings. Default is 24."), ECVF_Cheat);
  FlightLiftVar->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &UCheatSubsystem::ChangeFlightLift));

  // UConsole::RegisterConsoleAutoCompleteEntries.AddStatic(&UMyCheatManager::AddAutoCompleteEntries);

     //Add console variables here
     //...
     //...
     //...

} 


void UCheatSubsystem::EnableSandboxMode()
{
  UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
  UManagerSubsystem& ManagerSubsystem = UManagerSubsystem::Get(this);

  ManagerSubsystem.Avatar->Backpack->IncreaseStorageCapacity(60);
  if(ManagerSubsystem.EquipmentManager) ManagerSubsystem.EquipmentManager->Deconstructor = true;
  ManagerSubsystem.Avatar->VoiceOverComp->PlayedVOs.Add(TEXT("Forge")); //So the VO isn't played on game start

  TArray<FName> blockkeys = { TEXT("Block"), TEXT("Shaft"), TEXT("Anchor"), TEXT("Bend"), TEXT("FuelTank"), TEXT("Fan"), TEXT("Wheel"), TEXT("Plank"), TEXT("Blade"), TEXT("Pipe"),
                              TEXT("Conveyor"), TEXT("Cache"), TEXT("Roller"), TEXT("Transmitter"), TEXT("Miner"), TEXT("Turbine"), TEXT("FloodLight"), TEXT("LightPole"),
                              TEXT("LargeTank"), TEXT("MachineGun"), TEXT("Flamethrower"), TEXT("LandMine"), TEXT("Cockpit"), TEXT("Motor"), TEXT("SteeringJoint"), TEXT("Bearing"),
                              TEXT("Rail"), TEXT("GrinderMan"), TEXT("Saw"), TEXT("Podlight"), TEXT("Thruster"), TEXT("Wing"), TEXT("CrossShaft"), TEXT("FlamethrowerPortable"),
                              TEXT("HyperMail"), TEXT("FirePit") };
  TArray<FName> resourcekeys =  { TEXT("Wood"), TEXT("Sn"), TEXT("Cu"), TEXT("Fe"), TEXT("Al"),  TEXT("Glass"), TEXT("Ammo")};
  TArray<FName> resourcekeys2 = { TEXT("Butane"), TEXT("HyperMailStamp"), TEXT("Fuel"), TEXT("BatteryCharge") };
  TArray<FName> resourcekeys3 = { TEXT("Water"), TEXT("Coconut"), TEXT("Pb"), TEXT("Cassiterite"), TEXT("Bornite"), TEXT("Galena"), TEXT("Rope"), TEXT("FlareShell"), TEXT("ExplosiveShell"), TEXT("Mushroom"), TEXT("LeadAcidBattery")};
  TArray<FName> equipmentkeys = { TEXT("Forge"), TEXT("Axe"), TEXT("Lighter"), TEXT("FlareGun"), TEXT("TimeBender"), TEXT("Vortex")};
  TArray<FName> upgradekeys = { TEXT("Upgrade_Basics2"), TEXT("Upgrade_Automation") };
  TArray<FName> HyperMailKeys = { TEXT("TBenderCapsule1") };

  for (FName key : blockkeys)
  {
    ManagerSubsystem.BlockManager->UnlockBlock(key);
  }

  for (FName key : resourcekeys2)
  {
    ManagerSubsystem.Avatar->Backpack->AddToStorage(key, 2000);
  }

  for (FName key : resourcekeys3)
  {
    ManagerSubsystem.Avatar->Backpack->AddToStorage(key, 20);
  }

  for (FName key : resourcekeys)
  {
    ManagerSubsystem.Avatar->Backpack->AddToStorage(key, 100);
  }

  for (FName key : equipmentkeys)
  {  
    FPayloadKey payload = { key };
    MessageSubsystem.BroadcastMessage(FChannelTags::Get().EquipmentPickup, payload);
  }

  for (FName key : upgradekeys)
  {
    FPayloadKey payload = { key };
    MessageSubsystem.BroadcastMessage(FChannelTags::Get().UpgradePickup, payload);
  }

  MessageSubsystem.BroadcastMessage(FChannelTags::Get().HyperMail, FPayloadHyperMail(HyperMailKeys));


}


void UCheatSubsystem::ToggleSpectatorMode()
{
  APawn* CurrentPawn = UGameplayStatics::GetPlayerPawn(this, 0);
  if (!IsValid(SpectatorPawn))
  {
    Avatar = Cast<AAvatar>(CurrentPawn);
    SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(GetDefault<UManagerSettings>()->SpectatorPawn.LoadSynchronous(), FActorSpawnParameters());
  }

  APawn* PawnToPossess;
  if (CurrentPawn->GetClass() == Avatar->GetClass()) PawnToPossess = SpectatorPawn;
  else  PawnToPossess = Avatar;

  if(IsValid(PawnToPossess)) PawnToPossess->SetActorLocation(CurrentPawn->GetActorLocation());
  UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(PawnToPossess);

}

void UCheatSubsystem::SaveGame()
{
  USaveGameSubsystem::Get(this).WriteSaveGame();
}

void UCheatSubsystem::LoadGame()
{
  USaveGameSubsystem::Get(this).LoadSaveGame(true);
}

void UCheatSubsystem::SkipIntro()
{
   UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().PPExposure, FPayloadBool(true));
   Avatar = UManagerSubsystem::Get(this).Avatar;
   if (Avatar)
   {
     Avatar->EnableCharacterInput(true, false);
     Avatar->MovementComponent->LimitLook(false);
     Avatar->SetActorLocation(FVector(67150, 61895, 824));
   }
   
}

void UCheatSubsystem::Invincible()
{
  Avatar = UManagerSubsystem::Get(this).Avatar;
  if (Avatar)
  {
    Avatar->OverrideHealth(100.f, true);
  }

}

void UCheatSubsystem::Armin()
{
  SkipIntro();
  EnableSandboxMode();
  Invincible();
  ToggleSpectatorMode();
}


void UCheatSubsystem::ApplySkyConfig(IConsoleVariable* Var)
{
  int32 var = Var->GetInt();
  FName ConfigName = NAME_None;

  switch (var)
  {
  case 1:
    ConfigName = TEXT("PurpleDawn");
    break;

  case 2:
    ConfigName = TEXT("PurpleDawnForWakeUp");
    break;

  case 3:
    ConfigName = TEXT("WarmFoggySun");
    break;

  case 4:
    ConfigName = TEXT("ColdFoggySunset");
    break;

  case 5:
    ConfigName = TEXT("PrettyMoonlight");
    break;
  case 6:
    ConfigName = TEXT("ChronoHeavy");
    break;
  case 7:
    ConfigName = TEXT("Hell");
    break;
  case 8:
    ConfigName = TEXT("ArminsSpecial");
    break;

  default:
    break;
  }

  UManagerSubsystem::Get(this).SkyManager->ApplySkyConfigByName(ConfigName);
 // UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().Sky, FPayloadKey(ConfigName));

}

void UCheatSubsystem::ChangeSkyTime(IConsoleVariable* Var)
{
  float Time = Var->GetFloat();
  UManagerSubsystem::Get(this).SkyManager->ChangeTimeOfDay(Time);
}

void UCheatSubsystem::ChangeDayLength(IConsoleVariable* Var)
{
  float Length = Var->GetFloat();
  UManagerSubsystem::Get(this).SkyManager->ChangeDayLength(Length);
}

void UCheatSubsystem::ChangeFlightLift(IConsoleVariable* Var)
{
  float Lift = Var->GetFloat();
  UGlobalConstants::Get(this).SetFlightLift(Lift);
}









