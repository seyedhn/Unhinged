// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockSocket.generated.h"

class UBlockSMC;

UENUM(BlueprintType)
enum class EBlockSocketType : uint8
{
	Attach	 UMETA(DisplayName = "Attach"),
	Base  UMETA(DisplayName = "Base"),
	Pipe  UMETA(DisplayName = "Pipe"),
	Conveyor  UMETA(DisplayName = "Conveyor"),
	Rail UMETA(DisplayName = "Rail")
};

USTRUCT(Blueprintable)
struct FSocketData
{
	GENERATED_BODY();

	FSocketData()
		: Primitive(), Socket(TEXT(""))
	{}

	FSocketData(UBlockSMC* _Primitive, const FName& _Socket)
		: Primitive(_Primitive), Socket(_Socket)
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBlockSMC* Primitive;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Socket;

	bool IsSocketOfType(EBlockSocketType Type) const;

	bool IsSocketValid() const;

	int32 GetSocketInOutDirection() const;
	
	FSocketSaveData ToSaveData() const;
	
	bool operator==(const FSocketData& Other) const
	{
		return (Socket == Other.Socket && Primitive == Other.Primitive);
	}

};





USTRUCT()
struct FSocketSaveData
{
	GENERATED_BODY();

	FSocketSaveData()
	{}

	FSocketSaveData(FGuid& _ActorID, FName _CompName, FName _Socket)
		: ActorID(_ActorID), ComponentName(_CompName), Socket(_Socket)
	{}

	UPROPERTY(SaveGame)
	FGuid ActorID;

	UPROPERTY(SaveGame)
	FName ComponentName;

	UPROPERTY(SaveGame)
	FName Socket;

	bool operator==(const FSocketSaveData& Other) const
	{
		return (ActorID == Other.ActorID && ComponentName == Other.ComponentName && Socket == Other.Socket);
	}

};




USTRUCT()
struct FSocketPairData
{
	GENERATED_BODY();

	FSocketPairData()
	{}

	FSocketPairData(FSocketData _Socket1, FSocketData _Socket2)
		: Socket1(_Socket1.ToSaveData()), Socket2(_Socket2.ToSaveData())
	{}

	FSocketPairData(FSocketSaveData _Socket1, FSocketSaveData _Socket2)
		: Socket1(_Socket1), Socket2(_Socket2)
	{}

	UPROPERTY(SaveGame)
	FSocketSaveData Socket1;

	UPROPERTY(SaveGame)
	FSocketSaveData Socket2;

	bool operator==(const FSocketPairData& Other) const
	{
		return (Socket1 == Other.Socket1 && Socket2 == Other.Socket2) || (Socket1 == Other.Socket2 && Socket2 == Other.Socket1);
	}

	friend uint32 GetTypeHash(const FSocketPairData& pair)
	{
		return uint32(CityHash64((char*)&pair, sizeof(FSocketPairData)));
	}
};
