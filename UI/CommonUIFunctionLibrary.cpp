// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonUIFunctionLibrary.h"

#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/NavigationConfig.h"


void UCommonUIFunctionLibrary::EnableWASD_Navigation(bool Enable)
{
  TSharedRef<FNavigationConfig> navConfig = FSlateApplication::Get().GetNavigationConfig();
  TMap <FKey, EUINavigation>& KeysForFocus = navConfig->KeyEventRules;

  if (Enable)
  {  
  KeysForFocus.Emplace(EKeys::W, EUINavigation::Up);
  KeysForFocus.Emplace(EKeys::S, EUINavigation::Down);
  KeysForFocus.Emplace(EKeys::A, EUINavigation::Left);
  KeysForFocus.Emplace(EKeys::D, EUINavigation::Right);
  }
  else
  {
    KeysForFocus.Remove(EKeys::W);
    KeysForFocus.Remove(EKeys::S);
    KeysForFocus.Remove(EKeys::A);
    KeysForFocus.Remove(EKeys::D);
  }
  FSlateApplication::Get().SetNavigationConfig(navConfig);
}

void UCommonUIFunctionLibrary::EnableTabNavigation(bool Enable)
{
  TSharedRef<FNavigationConfig> navConfig = FSlateApplication::Get().GetNavigationConfig();
  navConfig->bTabNavigation = Enable;
}

void UCommonUIFunctionLibrary::InvertColour(UImage* Image)
{
  FLinearColor Color = FLinearColor(1.f, 1.f, 1.f, 1.f) - Image->GetColorAndOpacity();
  Color.A = 1.f;
  Image->SetColorAndOpacity(Color);
}

void UCommonUIFunctionLibrary::MakeIconBlack(UImage* Image)
{
  Image->SetColorAndOpacity(FLinearColor(0.f,0.f,0.f,1.f));
}

FString UCommonUIFunctionLibrary::DoubleDigit(int32 Time)
{
  FString TimeString = FString::FromInt(Time);
  if (TimeString.Len() == 1)
  {
    return FString("0").Append(TimeString);
  }
  return TimeString;
}

/*
void UCommonUIFunctionLibrary::SetMouseWheelAsNavKeys(bool Reset)
{
  TSharedRef<FNavigationConfig> navConfig = FSlateApplication::Get().GetNavigationConfig();
  TMap <FKey, EUINavigation>& KeysForFocus = navConfig->KeyEventRules;

  if (!Reset)
  {
    navConfig->AnalogNavigationVerticalThreshold = 0.0f;
    navConfig->bTabNavigation = false;
    KeysForFocus.Emplace(EKeys::LeftMouseButton, EUINavigation::Up);
    KeysForFocus.Emplace(EKeys::RightMouseButton, EUINavigation::Down);
    KeysForFocus.Emplace(EKeys::MouseScrollDown, EUINavigation::Down);
    navConfig->AnalogVerticalKey = EKeys::F;
  }
  else
  {
    navConfig->bTabNavigation = true;
    KeysForFocus.Remove(EKeys::MouseScrollUp);
    KeysForFocus.Remove(EKeys::MouseScrollDown);
  }
  FSlateApplication::Get().SetNavigationConfig(navConfig);
}*/


