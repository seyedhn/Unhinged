#include "OurUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "EngineSettings.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "AudioDevice.h"
#include "DLSSLibrary.h"
#include "HAL/IConsoleManager.h"
#include "Engine/AssetManager.h"


UOurUserSettings::UOurUserSettings()
{
  SetToDefaults();
}

void UOurUserSettings::SetToDefaults()
{
  Super::SetToDefaults();

  if (UAssetManager::GetIfValid())
  {
    LoadSettingDataAssets();
    SetDefaultValues();
  }

}

void UOurUserSettings::LoadSettingDataAssets()
{
  TArray<FAssetData> AssetData = UOurSystemLibrary::FindDataAssetsOfClass(USettingsPDA::StaticClass(), true, true, TEXT("/Game/OurContent/Blueprints/Interface/NewMenu/Settings/DataAssets"), true);
  for (auto& asset : AssetData)
  {
    SettingsDAs.Add(Cast<USettingsPDA>(asset.GetAsset()));
    //UE_LOG(LogTemp, Warning, TEXT("%s"), *UAssetManager::Get().GetPrimaryAssetIdForData(asset).PrimaryAssetName.ToString());
  }
}

void UOurUserSettings::SetDefaultValues()
{
  for (auto& DA : SettingsDAs)
  {
    for (auto& setting : DA->SettingProperties)
    {
      FName Name = setting.Get<FSettingInstancedProperty>().Name;
      if (setting.GetPtr<FSettingsBinary>()) UOurSystemLibrary::SetPropertyBool(this, Name, setting.Get<FSettingsBinary>().DefaultValue);
      if (setting.GetPtr<FSettingsScalability>()) UOurSystemLibrary::SetPropertyInt(this, Name, setting.Get<FSettingsScalability>().DefaultValue);
      if (setting.GetPtr<FSettingsMP>()) UOurSystemLibrary::SetPropertyInt(this, Name, setting.Get<FSettingsMP>().DefaultValue);
      if (setting.GetPtr<FSettingsSlider>()) UOurSystemLibrary::SetPropertyFloatNative(this, Name, setting.Get<FSettingsSlider>().DefaultValue);
    }
  }
}

void UOurUserSettings::ResetToDefault(const FInstancedStruct& setting)
{
  FName Name = setting.Get<FSettingInstancedProperty>().Name;
  if (setting.GetPtr<FSettingsBinary>()) SetBinary(Name, setting.Get<FSettingsBinary>().DefaultValue);
  if (setting.GetPtr<FSettingsMP>())SetMP(Name, setting.Get<FSettingsMP>().DefaultValue);
  if (setting.GetPtr<FSettingsScalability>()) SetScalability(Name, setting.Get<FSettingsScalability>().DefaultValue);
  if (setting.GetPtr<FSettingsSlider>()) SetSlider(Name, setting.Get<FSettingsSlider>().DefaultValue);
}


UOurUserSettings* UOurUserSettings::GetOurUserSettings()
{
  return Cast<UOurUserSettings>(UGameUserSettings::GetGameUserSettings());
}

void UOurUserSettings::InitialiseUserSettings(UWorld* _World)
{
  World = _World;
  WorldPP = GetWorldPP();
  FPCamera = GetFPCamera();
  UserSettingsActor = UManagerSubsystem::Get(World).UserSettingsActor;

  LoadSettings();
  ApplySettings(false);
  ApplySavedSettings();
}

void UOurUserSettings::ApplySavedSettings()
{
  for (auto& DA : SettingsDAs)
  {
    for (auto& setting : DA->SettingProperties)
    {
      ApplySavedSetting(setting);      
    }
  }
}

void UOurUserSettings::ApplySavedSetting(const FInstancedStruct& setting)
{
  FName Name = setting.Get<FSettingInstancedProperty>().Name;
  if (setting.GetPtr<FSettingsBinary>()) SetBinary(Name, GetBinary(Name));
  if (setting.GetPtr<FSettingsScalability>())  SetScalability(Name, GetMP(Name));
  if (setting.GetPtr<FSettingsMP>())  SetMP(Name, GetMP(Name));
  if (setting.GetPtr<FSettingsSlider>()) SetSlider(Name, GetSlider(Name));
}

bool UOurUserSettings::ApplyBinarySetting(const FInstancedStruct& Setting, const bool Value)
{
  FSettingsBinary setting = Setting.Get<FSettingsBinary>();
  SetBinary(setting.Name, Value);
  return setting.DefaultValue == Value;
}

bool UOurUserSettings::ApplySliderSetting(const FInstancedStruct& Setting, const float Value)
{
  FSettingsSlider setting = Setting.Get<FSettingsSlider>();
  SetSlider(setting.Name, Value);
  return setting.DefaultValue == Value;
}

bool UOurUserSettings::ApplyMPSetting(const FInstancedStruct& Setting, const int32 Value)
{
  FSettingsMP setting = Setting.Get<FSettingsMP>();
  SetMP(setting.Name, Value);
  return setting.DefaultValue == Value;
}

bool UOurUserSettings::ApplyScalabilitySetting(const FInstancedStruct& Setting, const int32 Value)
{
  FSettingsScalability setting = Setting.Get<FSettingsScalability>();
  SetScalability(setting.Name, Value);
  return setting.DefaultValue == Value;
}

bool UOurUserSettings::GetBinarySetting(const FInstancedStruct& Setting, bool& Value)
{
  FSettingsBinary setting = Setting.Get<FSettingsBinary>();
  Value = GetBinary(setting.Name);
  return setting.DefaultValue == Value;
}

bool UOurUserSettings::GetSliderSetting(const FInstancedStruct& Setting, float& Value)
{
  FSettingsSlider setting = Setting.Get<FSettingsSlider>();
  Value = GetSlider(setting.Name);
  return setting.DefaultValue == Value;
}

bool UOurUserSettings::GetMPSetting(const FInstancedStruct& Setting, int32& Value)
{
  FSettingsMP setting = Setting.Get<FSettingsMP>();
  Value = GetMP(setting.Name);
  return setting.DefaultValue == Value;
}

bool UOurUserSettings::GetScalabilitySetting(const FInstancedStruct& Setting, int32& Value)
{
  FSettingsScalability setting = Setting.Get<FSettingsScalability>();
  Value = GetMP(setting.Name);
  return setting.DefaultValue == Value;
}


void UOurUserSettings::ReloadSettings()
{
  FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([this] {OnReloadSettings.Broadcast();});
  World->GetTimerManager().SetTimerForNextTick(TimerDelegate);
}






//SETTINGS IMPLEMENTATIONS//

void UOurUserSettings::SetAAMethod(const int32 Value)
{
  if (FPCamera) FPCamera->SetOutlinePP(Value == 3);
  if (Value == 5)
  {
    ExecuteConsoleCommand("r.AntiAliasingMethod", FString::FromInt(4));
    UDLSSLibrary::EnableDLSS(true);
    ExecuteConsoleCommand("r.ScreenPercentage", "100");
    SetMP(TEXT("SSAAQuality"), 0);
    ReloadSettings();
    return;
  }

  UDLSSLibrary::EnableDLSS(false);
  ExecuteConsoleCommand("r.AntiAliasingMethod", FString::FromInt(Value));
}

void UOurUserSettings::SetVolumetricCloud(const bool Value)
{
  FString value = Value ? "1" : "0";
  ExecuteConsoleCommand("r.VolumetricCloud", value);
}

void UOurUserSettings::SetDFS(const bool Value)
{
  FString value = Value ? "1" : "0";
  ExecuteConsoleCommand("r.DistanceFieldShadowing", value);
}

void UOurUserSettings::SetMSAAQuality(const int32 Value)
{
  int32 MSAACount = 1;
  switch (Value)
  {
  case 0:
    MSAACount = 1;
    break;
  case 1:
    MSAACount = 2;
    break;
  case 2:
    MSAACount = 4;
    break;
  case 3:
    MSAACount = 8;
    break;
  }
  ExecuteConsoleCommand("r.MSAACount", FString::FromInt(MSAACount));
}

void UOurUserSettings::SetSSAAQuality(const int32 Value)
{
  int32 screenpercentage = 100;
  switch (Value)
  {
  case 0:
    screenpercentage = 100;
    break;
  case 1:
    screenpercentage = 141;
    break;
  case 2:
    screenpercentage = 173;
    break;
  case 3:
    screenpercentage = 200;
    break;
  }
  ExecuteConsoleCommand("r.ScreenPercentage", FString::FromInt(screenpercentage));
}

void UOurUserSettings::SetVolumetricFogQuality(const int32 Value)
{
  int32 Fog = 1;
  int32 PixelSize = 1;
  int32 SizeZ = 1;
  int32 HistoryCount = 1;
  switch (Value)
  {
  case 0:
    Fog = 0;
    break;
  case 1:
    Fog = 1;
    PixelSize = 32;
    SizeZ = 64;
    break;
  case 2:
    Fog = 1;
    PixelSize = 16;
    SizeZ = 96;
    break;
  case 3:
    Fog = 1;
    PixelSize = 8;
    SizeZ = 128;
    break;
  }
  ExecuteConsoleCommand("r.VolumetricFog", FString::FromInt(Fog));
  ExecuteConsoleCommand("r.VolumetricFog.GridPixelSize", FString::FromInt(PixelSize));
  ExecuteConsoleCommand("r.VolumetricFog.GridSizeZ", FString::FromInt(SizeZ));
}

void UOurUserSettings::SetLumen(const bool Value)
{
  FString value = Value ? "1" : "0";
  ExecuteConsoleCommand("r.DynamicGlobalIlluminationMethod", value);
}

void UOurUserSettings::SetMotionBlur(const bool Value)
{
  float Amount = Value ? 0.5f : 0.f;
  if(FPCamera) FPCamera->PostProcessSettings.MotionBlurAmount = Amount;
}

void UOurUserSettings::SetNaniteVSM(const bool Value)
{
  FString enabled = Value ? "1" : "0";
  ExecuteConsoleCommand("r.Nanite",enabled);
  ExecuteConsoleCommand("rse.Virtual.Enable", enabled);
}

void UOurUserSettings::SetGamma(const float Value)
{
  ExecuteConsoleCommand("gamma",  FString::SanitizeFloat(Value));
}

void UOurUserSettings::SetScreenMode(const int32 Value)
{
  EWindowMode::Type ScreenMode = EWindowMode::WindowedFullscreen;
  switch (Value)
  {
  case 0:
    ScreenMode = EWindowMode::Fullscreen;
    break;
  case 1:
    ScreenMode = EWindowMode::WindowedFullscreen;
    break;
  case 2:
    ScreenMode = EWindowMode::Windowed;
    break;
  }
  SetFullscreenMode(ScreenMode);
  ApplyResolutionSettings(false);
}

int32 UOurUserSettings::GetScreenMode()
{
  return FullscreenMode;
}

void UOurUserSettings::SetResolution(const int32 Value)
{
  FIntPoint Res = FIntPoint(1920, 1080);

  switch (Value)
  {
  case 0:
    Res = FIntPoint(1280, 720);
    break;
  case 1:
    Res = FIntPoint(1920, 1080);
    break;
  case 2:
    Res = FIntPoint(2560, 1440);
    break;
  case 3:
    Res = FIntPoint(3840, 2160);
    break;
  }
  SetScreenResolution(Res);
  ApplyResolutionSettings(false);
}

int32 UOurUserSettings::GetResolution()
{
  FIntPoint Res = GetScreenResolution();
  switch (Res.X)
  {
  case 1280:
    return 0;
  case 1920:
    return 1;
  case 2560:
    return 2;
  case 3840:
    return 3;
  }
  return 1;
}


void UOurUserSettings::SetRHI(const int32 Value)
{

  if(Value != RHI)
  {
    FString DefaultRHI;
    switch (Value)
    {
    case 0:
      DefaultRHI = "DefaultGraphicsRHI_DX11";
      break;
    case 1:
      DefaultRHI = "DefaultGraphicsRHI_DX12";
      break;
    case 2:
      DefaultRHI = "DefaultGraphicsRHI_Vulkan";
      break;
    }

    UEngineSettings* EngineSettings = GetMutableDefault<UEngineSettings>();
    EngineSettings->SetDefaultRHI(DefaultRHI);
  }
}

void UOurUserSettings::SetScreenMessages(const bool Value)
{
  if (Value)
  {
    GEngine->Exec(GetWorld(), TEXT("EnableAllScreenMessages"));
  }
  else
  {
    GEngine->Exec(GetWorld(), TEXT("DisableAllScreenMessages"));
  }
}

void UOurUserSettings::SetFOV(const float Value)
{
  if(FPCamera) FPCamera->SetFoV(Value);
}

void UOurUserSettings::SetFisheye(const bool Value)
{
  if(FPCamera) FPCamera->EnableFisheye(Value);
}

/*
void UOurUserSettings::SetMasterVolume(const float Value)
{
  UserSettingsActor->SetMasterVolume(Value);
}

void UOurUserSettings::SetAmbienceVolume(const float Value)
{
  UserSettingsActor->SetAmbienceVolume(Value);
}

void UOurUserSettings::SetVoiceVolume(const float Value)
{
  UserSettingsActor->SetVoiceVolume(Value);
}
*/
void UOurUserSettings::SetHeadBobbing(const bool Value)
{
  AAvatar* Avatar = UManagerSubsystem::Get(World).Avatar;
  if (IsValid(Avatar))
  {
    UMovementComp* MovementComp = Avatar->FindComponentByClass<UMovementComp>();
    MovementComp->bEnableHeadBobbing = Value;
    if (!Value) MovementComp->StopHeadBobbing();
  }
}

void UOurUserSettings::SetPostProcess(const bool Value)
{
    if(WorldPP) WorldPP->bEnabled = Value;
}

void UOurUserSettings::SetBlendWeight(const float Value)
{
  if (WorldPP) WorldPP->BlendWeight = Value;
}

void UOurUserSettings::SetColorGrading(const bool Value)
{
  FString val = Value ? "2" : "0";
  ExecuteConsoleCommand("ShowFlag.ColorGrading", val);
}

void UOurUserSettings::SetBloomMethod(const int32 Value)
{
  EBloomMethod Method = EBloomMethod::BM_SOG;
  switch (Value)
  {
  case 0:
    Method = EBloomMethod::BM_SOG;
    break;
  case 1:
    Method = EBloomMethod::BM_FFT;
    break;
  }
  if (WorldPP) WorldPP->Settings.BloomMethod = Method;
}

void UOurUserSettings::SetBloomIntensity(const float Value)
{
  if (WorldPP)
  {
    WorldPP->Settings.bOverride_BloomIntensity = true;
    WorldPP->Settings.BloomIntensity = Value;
  }
}

void UOurUserSettings::SetBloomThreshold(const float Value)
{
  if (WorldPP)
  {
  WorldPP->Settings.bOverride_BloomThreshold = true;
  WorldPP->Settings.BloomThreshold = Value;
  }
}

void UOurUserSettings::SetAOIntensity(const float Value)
{
  if (WorldPP)
  {
  WorldPP->Settings.bOverride_AmbientOcclusionIntensity = true;
  WorldPP->Settings.AmbientOcclusionIntensity = Value;
  }
}

void UOurUserSettings::SetAORadius(const float Value)
{
  if (WorldPP)
  {
  WorldPP->Settings.bOverride_AmbientOcclusionRadius = true;
  WorldPP->Settings.AmbientOcclusionRadius = Value;
  }
}

void UOurUserSettings::SetChromatic(const float Value)
{
  if (WorldPP)
  {
  WorldPP->Settings.bOverride_SceneFringeIntensity = true;
  WorldPP->Settings.SceneFringeIntensity = Value;
  }
}

void UOurUserSettings::SetDirtMask(const float Value)
{
  if (WorldPP)
  {
  WorldPP->Settings.bOverride_BloomDirtMaskIntensity = true;
  WorldPP->Settings.BloomDirtMaskIntensity = Value;
  }
}

void UOurUserSettings::SetVignette(const float Value)
{
  if (WorldPP)
  {
  WorldPP->Settings.bOverride_VignetteIntensity = true;
  WorldPP->Settings.VignetteIntensity = Value;
  }
}

void UOurUserSettings::SetGradingTemp(const float Value)
{
  if(WorldPP)
  {
  WorldPP->Settings.bOverride_WhiteTemp = true;
  WorldPP->Settings.WhiteTemp = Value;
  }
}

void UOurUserSettings::SetGradingTint(const float Value)
{
  if (WorldPP)
  {
  WorldPP->Settings.bOverride_WhiteTint = true;
  WorldPP->Settings.WhiteTint = Value;
  }
}

void UOurUserSettings::SetFilmSlope(const float Value)
{
  if (WorldPP)
  {
  WorldPP->Settings.bOverride_FilmSlope = true;
  WorldPP->Settings.FilmSlope = Value;
  }
}

void UOurUserSettings::SetToneCurve(const float Value)
{
  if (WorldPP)
  {
    WorldPP->Settings.bOverride_ToneCurveAmount = true;
    WorldPP->Settings.ToneCurveAmount = Value;
  }
}

void UOurUserSettings::SetFilmGrain(const float Value)
{
  if (WorldPP)
  {
  WorldPP->Settings.bOverride_FilmGrainIntensity = true;
  WorldPP->Settings.FilmGrainIntensity = Value;
  }
}

void UOurUserSettings::SetFilmGrainShadows(const float Value)
{
  if (WorldPP)
  {
    GetWorldPP()->Settings.bOverride_FilmGrainIntensityShadows = true;
    GetWorldPP()->Settings.FilmGrainIntensityShadows = Value;
  }
}

void UOurUserSettings::SetSkyPreset(const int32 Value)
{
  ExecuteConsoleCommand("dev.sky", FString::FromInt(Value + 1));
  ReloadSettings();
}











//SETTER AND GETTER HELPER FUNCIONS//

FName UOurUserSettings::Setter(const FName Name) const
{
  FString FuncName = FString("Set");
  FuncName.Append(Name.ToString());
  return FName(FuncName);
}

FName UOurUserSettings::Getter(const FName Name) const
{
  FString FuncName = FString("Get");
  FuncName.Append(Name.ToString());
  return FName(FuncName);
}


//TYPE SETTERS//

void UOurUserSettings::SetBinary(const FName Name, const bool Value)
{
  if (UOurSystemLibrary::HasFunction(this, Setter(Name))) UOurSystemLibrary::ExecuteFunctionBool(this, Setter(Name), Value);
  if (UOurSystemLibrary::HasFunction(UserSettingsActor, Setter(Name)))  UOurSystemLibrary::ExecuteFunctionBool(UserSettingsActor, Setter(Name), Value);
  UOurSystemLibrary::SetPropertyBool(this, Name, Value);
}

void UOurUserSettings::SetMP(const FName Name, const int32 Value)
{
  if (UOurSystemLibrary::HasFunction(this, Setter(Name))) UOurSystemLibrary::ExecuteFunctionInt(this, Setter(Name), Value);
  if (UOurSystemLibrary::HasFunction(UserSettingsActor, Setter(Name)))  UOurSystemLibrary::ExecuteFunctionInt(UserSettingsActor, Setter(Name), Value);
  UOurSystemLibrary::SetPropertyInt(this, Name, Value);
}

void UOurUserSettings::SetScalability(const FName Name, const int32 Value)
{
  if (UOurSystemLibrary::HasFunction(this, Setter(Name))) UOurSystemLibrary::ExecuteFunctionInt(this, Setter(Name), Value);
  if (UOurSystemLibrary::HasFunction(UserSettingsActor, Setter(Name)))  UOurSystemLibrary::ExecuteFunctionInt(UserSettingsActor, Setter(Name), Value);
  UOurSystemLibrary::SetPropertyInt(this, Name, Value);
  ApplyNonResolutionSettings();
}

void UOurUserSettings::SetSlider(const FName Name, const float Value)
{
  if (UOurSystemLibrary::HasFunction(this, Setter(Name))) UOurSystemLibrary::ExecuteFunctionFloat(this, Setter(Name), Value);
  if (UOurSystemLibrary::HasFunction(UserSettingsActor, Setter(Name))) UOurSystemLibrary::ExecuteFunctionFloat(UserSettingsActor, Setter(Name), Value);
  UOurSystemLibrary::SetPropertyFloatNative(this, Name, Value);
}

//TYPE GETTERS//

bool UOurUserSettings::GetBinary(const FName Name)
{
  if (UOurSystemLibrary::HasFunction(this, Getter(Name))) return UOurSystemLibrary::ReturnFunctionBool(this, Getter(Name));
  if (UOurSystemLibrary::HasFunction(UserSettingsActor, Getter(Name)))  return UOurSystemLibrary::ReturnFunctionBool(UserSettingsActor, Getter(Name));
  return UOurSystemLibrary::GetPropertyBool(this, Name);
}

int32 UOurUserSettings::GetMP(const FName Name)
{
  if (UOurSystemLibrary::HasFunction(this, Getter(Name))) return UOurSystemLibrary::ReturnFunctionInt(this, Getter(Name));
  if (UOurSystemLibrary::HasFunction(UserSettingsActor, Getter(Name)))  UOurSystemLibrary::ReturnFunctionInt(UserSettingsActor, Getter(Name));
  return UOurSystemLibrary::GetPropertyInt(this, Name);
}

float UOurUserSettings::GetSlider(const FName Name)
{
  if (UOurSystemLibrary::HasFunction(this, Getter(Name)))  return UOurSystemLibrary::ReturnFunctionFloat(this, Getter(Name));
  if (UOurSystemLibrary::HasFunction(UserSettingsActor, Getter(Name)))  return UOurSystemLibrary::ReturnFunctionFloat(UserSettingsActor, Getter(Name));
  return UOurSystemLibrary::GetPropertyFloat(this, Name);
}


//C++ HELPER FUNCTIONS

UFPCameraComp* UOurUserSettings::GetFPCamera()
{
  AAvatar* Avatar = UManagerSubsystem::Get(World).Avatar;
  if (IsValid(Avatar))
  {
    return Avatar->FindComponentByClass<UFPCameraComp>();
  }
  return nullptr;
}

APostProcessVolume* UOurUserSettings::GetWorldPP()
{
  TArray<AActor*> PPs;
  UGameplayStatics::GetAllActorsOfClassWithTag(World, APostProcessVolume::StaticClass(), TEXT("Main"), PPs);
  if (PPs.Num() > 0)
  {
    return Cast<APostProcessVolume>(PPs[0]);
  }
  return nullptr;
}

void UOurUserSettings::ExecuteConsoleCommand(const FString& CVar, const FString& Value)
{
  FString command = CVar;
  command.Append(" ");
  command.Append(Value);
  GEngine->Exec(GetWorld(), *command);
}













//BLUEPRINT HELPER FUNCTIONS//


/*
void UOurUserSettings::SetSoundVolume(USoundMix* InSoundMixModifier, USoundClass* InSoundClass, float Volume)
{
  if (FAudioDeviceHandle AudioDevice = World->GetAudioDevice())
  {
    AudioDevice->SetSoundMixClassOverride(InSoundMixModifier, InSoundClass, Volume, 1.f, 1.f, true);
    AudioDevice->PushSoundMixModifier(InSoundMixModifier);
  }
}

UUserWidget* UOurUserSettings::GetWidgetOfClass(TSubclassOf<UUserWidget> WidgetClass)
{
  TArray<UUserWidget*> Widgets;
  UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, Widgets, WidgetClass);
  if (Widgets.Num() == 0) return nullptr;
  return Widgets[0];
}

AActor* UOurUserSettings::GetActorOfClass(TSubclassOf<AActor> ActorClass)
{
  return UGameplayStatics::GetActorOfClass(World, ActorClass);
}

void UOurUserSettings::SetGamePaused(bool Pause)
{
  UGameplayStatics::SetGamePaused(World, Pause);
}
*/

/* DEFAULT VALUES
* 
  //SCALABILITY//
  Scalability::FQualityLevels QualityLevel = Scalability::FQualityLevels();
  QualityLevel.SetFromSingleQualityLevel(2);
  Scalability::SetQualityLevels(QualityLevel);

  //GRAPHICS//
  NaniteVSM = true;
  Lumen = false;
  AAMethod = 0;
  RHI = 1;
  DFS = true;
  NumShadowCascades = 2;
  MSAAQuality = 2;
  SSAAQuality = 0;
  VolumetricFogQuality = 2;
  VolumetricCloud = true;

  //CAMERA//
  MotionBlur = false;
  FOV = 90.f;
  HeadBobbing = true;
  Fisheye = false;

  //VIDEO//
  Gamma = 2.2f;
  PauseBG = 1.f;
  PauseGame = true;
  ScreenMessages = false;

  //AUDIO//
  MasterVolume = 1.f;
  VoiceVolume = 1.f;
  AmbienceVolume = 1.f;

  //POSTPROCESS//
  PostProcess = true;
  BlendWeight = 0.6f;
  ColorGrading = true;
  BloomMethod = 0;
  BloomIntensity = 3.4;
  BloomThreshold = 5.7;
  AOIntensity = 1.f;
  AORadius = 140.f;
  Chromatic = 0.6f;
  DirtMask = 5.7f;
  Vignette = 0.5f;
  GradingTemp = 7700.0f;
  GradingTint = 0.24f;
  ToneCurve = 0.91;
  FilmSlope = 0.77f;
  FilmGrain = 0.21f;
  FilmGrainShadows = 0.92f;
  */


