#include "OurButtonBase.h"
#include "ItemDataSubsystem.h"
#include "../ItemsData/ItemPropertyStructs.h"
#include "Kismet/GameplayStatics.h"
#include "CommonUIFunctionLibrary.h"

void UOurButtonBase::NativeOnInitialized()
{
  Super::NativeOnInitialized();
  Style = NormalButtonStyle;
}

void UOurButtonBase::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
  SetStyle(HoveredButtonStyle);
  bSelected = true;
  BroadcastItemSelected(bSelected);
}

void UOurButtonBase::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
  SetStyle(NormalButtonStyle);
  bSelected = false;
  BroadcastItemSelected(bSelected);
  UGameplayStatics::PlaySound2D(this, HoverSound);
}

void UOurButtonBase::NativeOnHovered()
{
  SetKeyboardFocus();
}

void UOurButtonBase::InitialiseButton(UOurGridBase* _ParentGrid, int32 _Index)
{
  ParentGrid = _ParentGrid;
  Index = _Index;
}

void UOurButtonBase::SetButtonData(FName ItemKey, int32 _ItemAmount, const FString& Symbol, const bool bUseLargeIcon)
{
  NumericalSymbol = Symbol;
  Key = ItemKey;

  bool ItemValid = !Key.IsNone();
  SetWidgetVisibilities(ItemValid);

  if (ItemValid)
  {   
    SetIcon(Key, bUseLargeIcon);
    Name = UItemDataSubsystem::Get(this).GetItemName(Key);
    if (ItemName) ItemName->SetText(Name);
    if (_ItemAmount != -1) UpdateAmount(_ItemAmount);
    SetAmountVisibility(_ItemAmount != -1);
  }

}

void UOurButtonBase::SetButtonDataTextOnly(FName ItemKey, int32 _ItemAmount, const FString& Symbol)
{
  NumericalSymbol = Symbol;
  Key = ItemKey;
  ItemName->SetText(UItemDataSubsystem::Get(this).GetItemName(Key));
  UpdateAmount(_ItemAmount);
}

void UOurButtonBase::SetIcon(FName ItemKey, bool bUseLargeIcon)
{
  if (bUseLargeIcon && UItemDataSubsystem::Get(this).ItemHasProperty<FItemInventoryData>(Key))
  {
    ItemImage->SetBrushFromSoftTexture(UItemDataSubsystem::Get(this).GetItemProperty<FItemInventoryData>(Key).LargeIcon);
  }
  else
  {
    ItemImage->SetBrushFromSoftTexture(UItemDataSubsystem::Get(this).GetItemIcon(Key));
  }
  UCommonUIFunctionLibrary::MakeIconBlack(ItemImage);
}

void UOurButtonBase::UpdateAmount(int32 NewAmount)
{
  FString text = NumericalSymbol;
  Amount = NewAmount;
  text.AppendInt(NewAmount);
  ItemAmountTextBlock->SetText(FText::FromString(text));
}

void UOurButtonBase::SetLocked()
{
  bItemLocked = true;
}

bool UOurButtonBase::IsLocked() const
{
  return bItemLocked;
}

void UOurButtonBase::BroadcastItemSelected(bool Selected)
{
  FName key = bSelected ? Key : NAME_None;
  if(ParentGrid) ParentGrid->OnItemSelected.Broadcast(key, Index, ParentGrid->GridIndex);
}


void UOurButtonBase::SetAmountVisibility(bool bVisible)
{
  ESlateVisibility visibility = bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
  if (ItemAmountTextBlock) ItemAmountTextBlock->SetVisibility(visibility);
  if (AmountBG) AmountBG->SetVisibility(visibility);
}

void UOurButtonBase::SetWidgetVisibilities(bool bVisible)
{
  ESlateVisibility visibility = bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
  SetAmountVisibility(bVisible);
  ItemImage->SetVisibility(visibility);
  if (ItemName) ItemName->SetVisibility(visibility);
}


