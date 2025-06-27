// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputCoreTypes.h"
#include "../Messaging/PayloadStructs.h"
#include "../Messaging/ChannelTags.h"
#include "../Save/SaveGameInterface.h"
#include "../Equipment/EquipmentBase.h"
#include "EquipmentHandler.generated.h"


class AWormhole;



UCLASS(Blueprintable, BlueprintType)
class THEGAME_API UEquipmentHandler : public UActorComponent, public ISaveGameInterface
{
	GENERATED_BODY()

public:

	friend class AEquipmentBase;

	UEquipmentHandler();

	UFUNCTION(BlueprintCallable)
	void AddEquipmentToFreeSlot(FName Key);

	UFUNCTION(BlueprintCallable)
	void RemoveEquipmentFromSlot(FName Key);

	UFUNCTION(BlueprintCallable)
	void SetQuickslot(FName Key, int32 Index);

	UFUNCTION(BlueprintCallable, meta =(ExpandBoolAsExecs = "ReturnValue"))
	bool IsItemInQuickslot(FName Key, int32& Index);

	UFUNCTION(BlueprintCallable)
	void UnequipWeapon(bool EquipLastWeapon);

	UFUNCTION(BlueprintCallable)
	void EquipGizmoToTab(uint8 TabIndex);

	UFUNCTION(BlueprintCallable)
	void EquipGizmoToTabWithPayload(const uint8 TabIndex, const FInstancedStruct& Payload);

	UFUNCTION(BlueprintCallable)
	void EquipWatchToTabWithPayload(const uint8 TabIndex, const FInstancedStruct& Payload);

	UFUNCTION(BlueprintCallable)
	void EquipForgeWithPayload(const uint8 TabIndex, const FInstancedStruct& Payload);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool InputToQuickslotMapping(int32& Index, FName& Item);

protected:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SwapEquipment();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PrepareSwapEquipment(FName EquipmentName);

	UFUNCTION()
	void EquipWithPayload(const FName EquipmentName, const uint8 TabIndex, const FInstancedStruct& _Payload);

public:

	UPROPERTY(BlueprintReadWrite, SaveGame, Category = "EquipmentHandler")
	TSet<FName> UnlockedWeapons;

	UPROPERTY(BlueprintReadWrite, SaveGame, Category = "EquipmentHandler")
	TArray<FName> Quickslots;

	UPROPERTY(BlueprintReadWrite, SaveGame, Category = "EquipmentHandler")
	FName EquippedWeaponName;

	UPROPERTY(BlueprintReadWrite, Category = "EquipmentHandler")
	AEquipmentBase* EquippedWeapon;

	UPROPERTY(BlueprintReadWrite, Category = "EquipmentHandler")
	bool SwappingEquipment;


protected:

	UPROPERTY(BlueprintReadWrite, Category = "EquipmentHandler")
	TArray<FKey> MappedKeys;

	UPROPERTY(BlueprintReadWrite, Category = "EquipmentHandler")
	FName LastHeldEquipment;

	UPROPERTY(BlueprintReadWrite, Category = "EquipmentHandler")
	FName EquipmentSwappingTo;

	UPROPERTY(BlueprintReadWrite, Category = "EquipmentHandler")
	uint8 EquipPayload;

	UPROPERTY(BlueprintReadWrite, Category = "EquipmentHandler")
	FInstancedStruct Payload;

public:

	//FLARE GUN//
	
	UPROPERTY(BlueprintReadWrite, SaveGame, Category = "FlareGun")
	FName CurrentShellType = TEXT("FlareShell");


	//FORGE PROPERTIES//
	UPROPERTY(BlueprintReadWrite, SaveGame, Category = "Forge")
	bool Deconstructor = false;


	//VORTEX PROPERTIES//

	UPROPERTY(BlueprintReadWrite, Category = "Vortex")
	AWormhole* DynamicWormhole;

	UPROPERTY(BlueprintReadWrite, SaveGame, Category = "Vortex")
	int32 VortexCapacity = 4;

	UPROPERTY(BlueprintReadWrite, SaveGame, Category = "Vortex")
	int32 VortexCharges = 2;

};
