// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentAnimPDA.h"
#include "../ItemsData/ItemPropertyStructs.h"
#include "EquipmentBase.generated.h"

class AAvatar;
class UFPCameraComp;


UENUM(BlueprintType)
enum class EEquipmentState : uint8
{
	Idle	 UMETA(DisplayName = "Idle"),
	Active  UMETA(DisplayName = "Active"),
	Running  UMETA(DisplayName = "Running"),
	Attack  UMETA(DisplayName = "Attack")
};


UCLASS(Blueprintable, BlueprintType)
class THEGAME_API AEquipmentBase : public AActor
{
	GENERATED_BODY()

public:

	friend class UEquipmentHandler;

	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable, BlueprintPure)
	FName GetName() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EEquipmentState GetEquipmentState() const;

protected:

	//INPUT ACTIONS//

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void MouseWheel(int32 Direction);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void LMB_Triggered(float ElapsedTime);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void LMB_Started();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void LMB_Released(float ElapsedTime);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void RMB_Triggered();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void RMB_Released();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void LMBRMBCombo(float ElapsedTime);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Q_Pressed();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void R_Pressed();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Fast_R_Pressed();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Enter_Pressed();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Del_Pressed();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void T_Pressed();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void F_Pressed();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void E_Triggered(float ElapsedTime);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void E_Started();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void E_Released();


	//EVENTS//

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Equip(const uint8 EquipPayload, const FInstancedStruct& Payload);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Unequip();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Recoil();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void DealWeaponDamageOnAnimNotify();

	UFUNCTION(BlueprintCallable)
	void UpdateState(EEquipmentState State);

	//PROPERTIES//

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	FName Name;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	AAvatar* Avatar;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	UFPCameraComp* FirstPersonCamera;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UEquipmentAnimPDA* Anim_DA;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TSubclassOf<UAnimInstance> AnimLayer;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	EEquipmentType Type;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	EEquipmentState EquipmentState;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	FItemDamager DamagerProperties;

};
