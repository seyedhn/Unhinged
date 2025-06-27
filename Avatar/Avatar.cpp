#include "Avatar.h"
#include "../Manager/ManagerSettings.h"


AAvatar::AAvatar()
{
  RecallComp = CreateDefaultSubobject<URecallComp>(FName(TEXT("RecallComp")));
  HallucinationComp = CreateDefaultSubobject<UHallucinationComp>(FName(TEXT("HallucinationComp")));
  VitalsComp = CreateDefaultSubobject<UVitalsComp>(FName(TEXT("VitalsComp")));
  VoiceOverComp = CreateDefaultSubobject<UVoiceOverComp>(FName(TEXT("VoiceOverComp")));
  Backpack = CreateDefaultSubobject<UStorageComp>(FName(TEXT("Backpack")));

  UClass* EquipmentHandlerClass = GetDefault<UManagerSettings>()->EquipmentHandler.Get();
  //UClass* FPCameraClass = GetDefault<UManagerSettings>()->FPCameraComp.Get();
  EquipmentHandler = Cast<UEquipmentHandler>(CreateDefaultSubobject(FName(TEXT("EquipmentHandler")), EquipmentHandlerClass, EquipmentHandlerClass, false, false));
  //FPCamera = Cast<UFPCameraComp>(CreateDefaultSubobject(FName(TEXT("FPCamera")), FPCameraClass, FPCameraClass, false, false));


  UClass* MovementCompClass = GetDefault<UManagerSettings>()->MovementComp.Get();
  MovementComponent = Cast<UMovementComp>(CreateDefaultSubobject(FName(TEXT("MovementComponent")), MovementCompClass, MovementCompClass, false, false));

  Backpack->SetStorageCapacity(16);
}

void AAvatar::OverrideHealth(float Health, bool Invincible)
{
  VitalsComp->BecomeInvincible(Invincible);
  VitalsComp->Health = Health;
}
