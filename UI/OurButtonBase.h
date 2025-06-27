// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "OurGridBase.h"
#include "OurButtonBase.generated.h"

/**
 * 
 */
UCLASS()
class THEGAME_API UOurButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized();
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent);
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent);
	virtual void NativeOnHovered() override;

	void SetAmountVisibility(bool bVisible);
	void SetWidgetVisibilities(bool bVisible);

	UFUNCTION(BlueprintCallable)
	void InitialiseButton(UOurGridBase* _ParentGrid, int32 Index);

	UFUNCTION(BlueprintCallable)
	virtual void SetButtonData(FName ItemKey, int32 ItemAmount = -1, const FString& Symbol = "", const bool bUseLargeIcon = false);

	UFUNCTION(BlueprintCallable)
	void SetButtonDataTextOnly(FName ItemKey, int32 ItemAmount, const FString& Symbol);
	void SetIcon(FName ItemKey, bool bUseLargeIcon);

	UFUNCTION(BlueprintCallable)
	void UpdateAmount(int32 NewAmount);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateBoxVisuals(bool bNegative, int32 Numeric, const FString& Symbol);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ApplyLockedVisuals();

	UFUNCTION(BlueprintCallable)
	void SetLocked();

	UFUNCTION(BlueprintCallable)
	bool IsLocked() const;

protected:

	UFUNCTION(BlueprintCallable)
	void BroadcastItemSelected(bool Selected);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UCommonButtonStyle> NormalButtonStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UCommonButtonStyle> HoveredButtonStyle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USoundBase* HoverSound;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidgetOptional))
	UOurGridBase* ParentGrid;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidgetOptional))
	UImage* ItemImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidgetOptional))
	UCommonTextBlock* ItemAmountTextBlock;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidgetOptional))
	UCommonTextBlock* OptionalText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidgetOptional))
	UCommonTextBlock* ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidgetOptional))
	UImage* AmountBG;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Index;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Key;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Amount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString NumericalSymbol;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bSelected;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bItemLocked;





};