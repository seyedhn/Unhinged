// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameObject.generated.h"



USTRUCT()
struct FComponentSaveData
{
	GENERATED_BODY()

public:

	FComponentSaveData()
		: ComponentName(NAME_None), ComponentTransform(FTransform()), IsSimulating(false)
	{}

	FComponentSaveData(FName _CompName)
		: ComponentName(_CompName), ComponentTransform(FTransform()), IsSimulating(false)
	{}

	FComponentSaveData
	(FName _CompName, UClass* _CompClass, FTransform _CompTransform, bool _IsSimulating)
		: ComponentName(_CompName), ComponentClass(_CompClass), ComponentTransform(_CompTransform), IsSimulating(_IsSimulating)
	{}

	UPROPERTY()
	FName ComponentName;

	UPROPERTY()
	UClass* ComponentClass = nullptr;

	UPROPERTY()
	FTransform ComponentTransform;

	UPROPERTY()
	bool IsSimulating;

	UPROPERTY()
	TArray<uint8> ComponentByteData;

	bool operator==(const FComponentSaveData& Other) const
	{
		return ComponentName.IsEqual(Other.ComponentName);
	}

	friend uint32 GetTypeHash(const FComponentSaveData& SaveComp)
	{
		return uint32(CityHash64((char*)&SaveComp.ComponentName, sizeof(FName)));
	}

};


USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:

	FActorSaveData()
	{}

	FActorSaveData(FGuid& _ActorID)
		: ActorID(_ActorID)
	{}

	UPROPERTY()
	FGuid ActorID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	TArray<uint8> ByteData;

	UPROPERTY()
	UClass* ActorClass = nullptr;

	UPROPERTY()
	TSet<FComponentSaveData> Components;

	bool operator==(const FActorSaveData& Other) const
	{
		return (ActorID == Other.ActorID);
	}

	friend uint32 GetTypeHash(const FActorSaveData& SaveData)
	{
		return uint32(CityHash64((char*)&SaveData.ActorID, sizeof(FGuid)));
	}

};


USTRUCT()
struct FPlayerSaveData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FTransform PlayerTransform;

	UPROPERTY()
	FRotator ControllerRot = FRotator(0.f);

	UPROPERTY()
	TSet<FComponentSaveData> Components;


};


USTRUCT()
struct FManagerSaveData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<uint8> ManagerByteData;

};


USTRUCT()
struct FFoliageCompSaveData
{
	GENERATED_BODY()

public:

	FFoliageCompSaveData()
	{}

	FFoliageCompSaveData(TArray<int32> _RemovedInstances)
		: RemovedInstances(_RemovedInstances)
	{}

	UPROPERTY()
	TArray<int32> RemovedInstances;

	UPROPERTY()
	TArray<uint8> FoliageByteData;


};



USTRUCT()
struct FFoliageSaveData
{
	GENERATED_BODY()

public:

	FFoliageSaveData()
	{}

	FFoliageSaveData(FName _ActorName)
		: ActorName(_ActorName)
	{}

	FFoliageSaveData(FName _ActorName, TMap<FName, FFoliageCompSaveData>& _FoliageComps)
		: ActorName(_ActorName), FoliageComps(_FoliageComps)
	{}

	UPROPERTY()
	FName ActorName;

	UPROPERTY()
	TMap<FName, FFoliageCompSaveData> FoliageComps;

	bool operator==(const FFoliageSaveData& Other) const
	{
		return (ActorName == Other.ActorName);
	}

	friend uint32 GetTypeHash(const FFoliageSaveData& SaveData)
	{
		return uint32(CityHash64((char*)&SaveData.ActorName, sizeof(FName)));
	}
};



UCLASS()
class THEGAME_API USaveGameObject : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	TSet<FActorSaveData> SavedActors;

	UPROPERTY()
	FPlayerSaveData SavedPlayer;

	UPROPERTY()
	TMap<UClass*, FManagerSaveData> SavedManagers;

	UPROPERTY()
	TSet<FFoliageSaveData> SavedFoliage;

};



