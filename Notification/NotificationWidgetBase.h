// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationWidgetBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemoveDelegate, UNotificationWidgetBase*, Widget);

UENUM(BlueprintType)
enum class ENotificationType : uint8
{
	Notification	 UMETA(DisplayName = "Notification"),
	Objective  UMETA(DisplayName = "Objective"),
};


UCLASS()
class THEGAME_API UNotificationWidgetBase : public  UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowWidget();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void RemoveWidget();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShiftWidget();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShiftReverse();


	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FRemoveDelegate Removed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Key;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	ENotificationType Type;

};