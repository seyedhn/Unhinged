// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameFramework/SpectatorPawn.h"
#include "../Avatar/Avatar.h"
#include "CheatSubsystem.generated.h"

class IConsoleVariable;

UCLASS(Blueprintable)
class THEGAME_API UCheatSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	friend class UManagerSubsystem;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	static UCheatSubsystem& Get(UObject* WorldContextObject);
	void RegisterConsoleCommands();

	UFUNCTION(BlueprintCallable)
	void EnableSandboxMode();
	UFUNCTION(BlueprintCallable)
	void ToggleSpectatorMode();
	UFUNCTION()
	void SaveGame();
	UFUNCTION()
	void LoadGame();
	UFUNCTION()
	void SkipIntro();
	UFUNCTION()
	void Invincible();
	UFUNCTION()
	void Armin();

	void ApplySkyConfig(IConsoleVariable* Var);
	void ChangeSkyTime(IConsoleVariable* Var);
	void ChangeDayLength(IConsoleVariable* Var);
	void ChangeFlightLift(IConsoleVariable* Var);

//  TAutoConsoleVariable<int32> Flags;
//	UPROPERTY(meta=(ConsoleVariable = "dev.loc"))
//	int32 LocVar = 0;


private:
	UPROPERTY()
	ASpectatorPawn* SpectatorPawn;
	UPROPERTY()
	AAvatar* Avatar;
};



