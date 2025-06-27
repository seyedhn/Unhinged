#include "EngineSettings.h"

void UEngineSettings::SetDefaultRHI(FString RHI)
{
  GConfig->LoadFile(*GetDefaultConfigFilename());
  LoadConfig();

  GConfig->SetString(TEXT("/Script/WindowsTargetPlatform.WindowsTargetSettings"), TEXT("DefaultGraphicsRHI"), *RHI, *GetDefaultConfigFilename());

  GConfig->Flush(true, *GetDefaultConfigFilename());
  //SaveConfig(CPF_Config, *GetDefaultConfigFilename());
  bool success = TryUpdateDefaultConfigFile(*GetDefaultConfigFilename(), true);

   
  FString DefaultGraphicsRHI;
  FString aaa = success ? "success" : "fail";
  GConfig->GetString(TEXT("/Script/WindowsTargetPlatform.WindowsTargetSettings"), TEXT("DefaultGraphicsRHI"), DefaultGraphicsRHI, *GetDefaultConfigFilename());
  UE_LOG(LogTemp, Warning, TEXT("%s"), *DefaultGraphicsRHI);
  UE_LOG(LogTemp, Warning, TEXT("%s"), *GetDefaultConfigFilename());
  UE_LOG(LogTemp, Warning, TEXT("%s"), *aaa);
}
