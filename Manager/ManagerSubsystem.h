// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Resource/ResourceManager.h"
#include "../Block/BlockManager.h"
#include "../Damage/DamageManager.h"
#include "../Audio/AudioManager.h"
#include "../Heat/CombustionManager.h"
#include "../Heat/HeatManager.h"
#include "../Notification/NotificationManager.h"
#include "../Upgrade/UpgradeManager.h"
#include "../Spline/PipeManager.h"
#include "../Sky/SkyManager.h"
#include "../Postprocess/PostprocessManager.h"
#include "../Storage/StorageCentral.h"
#include "../Grid/GridManager.h"
#include "../Avatar/Avatar.h"
#include "../Rewind/RewindManager.h"
#include "../Trials/TrialsManager.h"
#include "../UserSettings/UserSettingsActor.h"
#include "ManagerBase.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ManagerSubsystem.generated.h"


UCLASS(Blueprintable)
class THEGAME_API UManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	static UManagerSubsystem& Get(const UObject* WorldContextObject);
	static UManagerSubsystem& Get(const UWorld* World);

	UFUNCTION(BlueprintCallable)
	void InitialiseManagerActors();

	UPROPERTY(BlueprintReadOnly)
	AResourceManager* ResourceManager;

	UPROPERTY(BlueprintReadOnly)
	ADamageManager* DamageManager;

	UPROPERTY(BlueprintReadOnly)
	ABlockManager* BlockManager;

	UPROPERTY(BlueprintReadOnly)
	AUpgradeManager* UpgradeManager;

	UPROPERTY(BlueprintReadOnly)
	APipeManager* PipeManager;

	UPROPERTY(BlueprintReadOnly)
	APostprocessManager* PostprocessManager;

	UPROPERTY(BlueprintReadOnly)
	AStorageCentral* CentralStorage;

	UPROPERTY(BlueprintReadOnly)
	AAvatar* Avatar;

	UPROPERTY(BlueprintReadOnly)
	UEquipmentHandler* EquipmentManager;

	UPROPERTY(BlueprintReadOnly)
	AGridManager* GridManager;

	UPROPERTY(BlueprintReadOnly)
	AAudioManager* AudioManager;

	UPROPERTY(BlueprintReadOnly)
	ACombustionManager* CombustionManager;

	UPROPERTY(BlueprintReadOnly)
	AHeatManager* HeatManager;

	UPROPERTY(BlueprintReadOnly)
	ANotificationManager* NotificationManager;

	UPROPERTY(BlueprintReadOnly)
	ARewindManager* RewindManager;

	UPROPERTY(BlueprintReadOnly)
	ASkyManager* SkyManager;

	UPROPERTY(BlueprintReadOnly)
	ATrialsManager* TrialsManager;

	UPROPERTY(BlueprintReadOnly)
	AUserSettingsActor* UserSettingsActor;

	//Add Manager references here
	//..
	//..
	//..

	UPROPERTY(BlueprintReadWrite)
	bool SandboxMode;

private:


};



