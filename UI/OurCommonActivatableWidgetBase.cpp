// Fill out your copyright notice in the Description page of Project Settings.


#include "OurCommonActivatableWidgetBase.h"

void UOurCommonActivatableWidgetBase::NativeConstruct()
{
  bAutomaticallyRegisterInputOnConstruction = bRegisterEnhancedInput;
  Super::NativeConstruct();

  /*
  TSharedRef<FNavigationConfig> navConfig = FSlateApplication::Get().GetNavigationConfig();
  TMap <FKey, EUINavigation>& KeysForFocus = navConfig->KeyEventRules;

  navConfig->bKeyNavigation = true;
  navConfig->bTabNavigation = false;

  KeysForFocus.Emplace(EKeys::W, EUINavigation::Up);
  KeysForFocus.Emplace(EKeys::S, EUINavigation::Down);
  KeysForFocus.Emplace(EKeys::A, EUINavigation::Left);
  KeysForFocus.Emplace(EKeys::D, EUINavigation::Right);
  KeysForFocus.Remove(EKeys::Tab);

  FSlateApplication::Get().SetNavigationConfig(navConfig);
  */
 // UpDelegate.BindDynamic(this, &UUW\_PlayerMenu::Up);
 // SetNavigationRuleCustom(EUINavigation::Up, UpDelegate);
}

void UOurCommonActivatableWidgetBase::NativeOnInitialized()
{
	//bAutomaticallyRegisterInputOnConstruction = bRegisterEnhancedInput;
	Super::NativeOnInitialized();
}
