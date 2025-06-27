// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Manager/ManagerBase.h"
#include "../Messaging/PayloadStructs.h"
#include "../Messaging/ChannelTags.h"
#include "../Trials/TrialsPDA.h"
#include "UpgradeManager.generated.h"

UENUM(BlueprintType)
enum class EUpgradeType : uint8
{
	None  UMETA(DisplayName = "None"),
	Block	 UMETA(DisplayName = "Block"),
	Tier  UMETA(DisplayName = "Tier"),
	Inventory UMETA(DisplayName = "Inventory"),
	EquipmentUpgrade UMETA(DisplayName = "EquipmentUpgrade")
};

UENUM(BlueprintType)
enum class EPurchaseType : uint8
{
	Consumable	 UMETA(DisplayName = "Consumable"),
	Sellable UMETA(DisplayName = "Sellable"),
	Upgrade  UMETA(DisplayName = "Upgrade")
};



UCLASS(Blueprintable, BlueprintType)
class THEGAME_API AUpgradeManager : public AManagerBase
{
	GENERATED_BODY()

public:

	friend class UManagerSubsystem;
	friend class UCheatSubsystem;

	AUpgradeManager();

	void BeginPlay() override;

	//---------------SCRIPTS------------------//

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsScriptInstalled(const FName Key) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsScriptUnlocked(const FName Key) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsUpgradeOrdered(const FName Key) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool AreThereUninstalledScripts() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetNumOrderedUpgrades() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSet<FName> GetOrderedUpgrades() const;

	UFUNCTION(BlueprintCallable)
	void InstallScript(const FName ScriptKey);


	//---------------TIER----------------------//

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsTierValid(const int32 Tier) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsTierPlusOne(const int32 Tier, const bool IncludeLowers) const;

	UFUNCTION(BlueprintCallable)
	void IncrementTier();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetCurrentTier() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetCurrentTierPoints() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetTierPoints(int32 Tier) const;

	//--------------TRIALS---------------------//

	UFUNCTION(BlueprintCallable)
	TMap<FName, int32> GetAvailableRewards(const FName TrialKey, const int32 MaxRewards) const;

	UFUNCTION(BlueprintCallable)
	bool IsTrialRewardAvailable(const FName Reward) const;

	UFUNCTION(BlueprintCallable)
	bool IsTrialTierValid(const FName TrialKey, int32& MinTier) const;

	UFUNCTION(BlueprintCallable)
	void IncrementTrialPoints(const int32 Amount);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetTotalTrialPoints() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetTrialPointsInCurrentTier() const;

	//------------------CREDITS--------------------//

	UFUNCTION(BlueprintCallable)
	void UpdateHyperMailCredit(int32 Change);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetHyperMailCredits() const;


private:
	void OnScriptPickup(FGameplayTag Channel, const FPayloadKey& Payload);
	void OnUpgradeOrdered(FGameplayTag Channel, const FPayloadHyperMail& Payload);
	void OnEquipmentPickup(FGameplayTag Channel, const FPayloadKey& Payload);
	void OnRewardClaimed(FGameplayTag Channel, const FPayloadKeyInt& Payload);


	//------------SCRIPT INSTALLS--------------------//
	UFUNCTION()
	void Upgrade_Inventory1();
	UFUNCTION()
	void Upgrade_Deconstructor();
	UFUNCTION()
	void Upgrade_TBenderCapsule2();

	//SCRIPTS//
	UPROPERTY(SaveGame)
	TSet<FName> InstalledScripts;

	UPROPERTY(SaveGame)
	TSet<FName> UnlockedScripts;// = { TEXT("Upgrade_Basics") };

	UPROPERTY(SaveGame)
	TSet<FName> OrderedUpgrades;

	//TRIALS//
	UPROPERTY()
	UTrialsPDA* TrialTiersDA;

	UPROPERTY(SaveGame)
	int32 CurrentTier;

	UPROPERTY(SaveGame)
	int32 TotalTrialPoints;

	UPROPERTY(SaveGame)
	TMap<int32, FTrialRewards> ClaimedRewards;

	//CREDITS
	UPROPERTY(SaveGame)
	int32 HyperMailCredits;

};
