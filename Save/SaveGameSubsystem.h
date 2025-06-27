// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGameObject.h"
#include "Subsystems/WorldSubsystem.h"
#include "SaveGameSubsystem.generated.h"



UCLASS(meta=(DisplayName="SaveGame Subsystem"))
class THEGAME_API USaveGameSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	static USaveGameSubsystem& Get(UObject* WorldContextObject);



	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	bool DoesSaveGameExist() const;
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void DeleteSaveGame();
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void RenameSaveGame();

	//SAVE//
	void SavePlayer();
	void SaveFoliage();
	void SaveManagers();
	void SaveActors();
	void SaveActorData(AActor* Actor, FActorSaveData& ActorData);
	void SaveActorComponents(AActor* Actor, TSet<FComponentSaveData>& ActorData);
	void SaveByteData(UObject* Object,TArray<uint8>& Data);

	//LOAD//
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void LoadSaveGame(bool RewindLoad = false);
	void LoadPlayer(bool RewindLoad);
	void LoadFoliage(bool RewindLoad);
	void LoadManagers(bool RewindLoad) const;
	void LoadActors(bool RewindLoad);
	void LoadActor(AActor* Actor, const FActorSaveData& ActorData, bool RewindLoad);
	void LoadActorComponents(AActor* Actor, const TSet<FComponentSaveData>& ActorData, bool RewindLoad = false);
	void LoadByteData(UObject* Object, const TArray<uint8>& Data) const;

	//ACTOR//
	void DestroyActor(AActor* ActorToDestroy);
	void SpawnActors(TSet<FActorSaveData>& SetOfActorsToSpawn, bool RewindLoad);
	void SpawnComponents(AActor* Owner, TSet<FComponentSaveData>& SetOfComponentsToSpawn, bool RewindLoad);
	AActor* GetActorFromID(const FGuid& ActorID);



	UPROPERTY(BlueprintReadOnly)
	FString CurrentSlotName;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USaveGameObject> CurrentSaveGame;

private:

	TMap<FGuid, AActor*> ActorIDs;

};



