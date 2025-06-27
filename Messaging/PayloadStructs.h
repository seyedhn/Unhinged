#pragma once

#include "CoreMinimal.h"
#include "../Block/BlockActor.h"
#include "../Resource/ResourceManager.h"
#include "../Postprocess/FPCameraComp.h"
#include "../Block/BlockSocket.h"
#include "../Resource/StorageComp.h"
#include "PayloadStructs.generated.h"



USTRUCT(Blueprintable)
struct FPayloadEmpty
{
	GENERATED_BODY()

	FPayloadEmpty() {}
};

USTRUCT(Blueprintable)
struct FPayloadKey
{
	GENERATED_BODY();

	FPayloadKey() {}
	FPayloadKey(FName _Key)
		: Key(_Key)
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FName Key;
};

USTRUCT(Blueprintable)
struct FPayloadInt
{
	GENERATED_BODY();

	FPayloadInt() {}
	FPayloadInt(int32 _Amount)
		: Amount(_Amount)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	int32 Amount = 0;
};

USTRUCT(Blueprintable)
struct FPayloadObject
{
	GENERATED_BODY();

	FPayloadObject() {}
	FPayloadObject(UObject* _Object)
		: Object(_Object)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	UObject* Object = nullptr;
};

USTRUCT(Blueprintable)
struct FPayloadActor
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	AActor* Actor = nullptr;
};

USTRUCT(Blueprintable)
struct FPayloadPrimitive
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	UPrimitiveComponent* Primitive = nullptr;
};

USTRUCT(Blueprintable)
struct FPayloadKeyInt
{
	GENERATED_BODY();

	FPayloadKeyInt() {}

	FPayloadKeyInt(FName _Key, int32 _Amount)
		: Key(_Key), amount(_Amount)
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FName Key;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	int32 amount = 0;
};



USTRUCT(Blueprintable)
struct FPayloadKeyBool
{
	GENERATED_BODY();

	FPayloadKeyBool() {}

	FPayloadKeyBool(FName _Key, bool _Activate)
		: Key(_Key), Activate(_Activate)
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FName Key;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	bool Activate = false;
};

USTRUCT(Blueprintable)
struct FPayloadBool
{
	GENERATED_BODY();

	FPayloadBool(){}
	FPayloadBool(bool _Activate)
		: Activate(_Activate)
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	bool Activate = false;
};


USTRUCT(Blueprintable)
struct FPayloadKeyObject
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FName Key;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	UPrimitiveComponent* Primitive = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	AActor* Actor = nullptr;
};


USTRUCT(Blueprintable)
struct FPayloadKeyBlock
{
	GENERATED_BODY();

	FPayloadKeyBlock() {}
	FPayloadKeyBlock(FName _Key, ABlockActor* _Block)
		: Key(_Key)
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FName Key;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	ABlockActor* Block = nullptr;
};


USTRUCT(Blueprintable)
struct FPayloadInventory
{
	GENERATED_BODY();

	FPayloadInventory() {}
	FPayloadInventory(FName _Key, int32 _Amount, UStorageComp* _Storage)
		: Key(_Key), Amount(_Amount), Storage(_Storage)
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FName Key;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	int32 Amount = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	UStorageComp* Storage = nullptr;
};


USTRUCT(Blueprintable)
struct FPayloadFlow
{
	GENERATED_BODY();

	FPayloadFlow() {}
	FPayloadFlow(FName _Key, ABlockActor* _Block, ABlockActor* _PreviousBlock)
		: Key(_Key), CurrentBlock(_Block), PreviousBlock(_PreviousBlock)
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FName Key;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	ABlockActor* CurrentBlock = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	ABlockActor* PreviousBlock = nullptr;
};


USTRUCT(Blueprintable)
struct FPayloadNotification
{
	GENERATED_BODY();

	FPayloadNotification() {}

	FPayloadNotification(FName _Key)
		: Key(_Key)
	{}

	FPayloadNotification(FText _NameOverride)
		: NameOverride(_NameOverride)
	{}

	FPayloadNotification(FName _Key, FText _Description)
		: Key(_Key), Description(_Description)
	{}

	FPayloadNotification(FText _NameOverride, FText _Description)
		:  Description(_Description), NameOverride(_NameOverride)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FName Key;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FText Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FText NameOverride;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TSoftObjectPtr<UTexture2D> IconOverride;
};



USTRUCT(Blueprintable)
struct FPayloadSpawnComp
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FName Key;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FTransform Transform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FVector Impulse = FVector(0.f);
};


USTRUCT()
struct FPayloadKeyIntMap
{
	GENERATED_BODY();

	FPayloadKeyIntMap() {}

	FPayloadKeyIntMap(TMap<FName, int32> _Deliveries)
		: Deliveries(_Deliveries)
	{}

	TMap<FName, int32> Deliveries;

};

USTRUCT()
struct FPayloadHyperMail
{
	GENERATED_BODY();

	FPayloadHyperMail(){}

	FPayloadHyperMail(FName _OrderedUpgrades)
		: Upgrades(TArray<FName>{_OrderedUpgrades})
	{}

	FPayloadHyperMail(TArray<FName> _OrderedUpgrades)
		: Upgrades(_OrderedUpgrades)
	{}

	FPayloadHyperMail(TMap<FName, int32> _Deliveries, TArray<FName> _OrderedUpgrades)
		: Consumables(_Deliveries), Upgrades(_OrderedUpgrades)
	{}

	TMap<FName, int32> Consumables;

	TArray<FName> Upgrades;
};


USTRUCT(Blueprintable)
struct FPayloadDetection
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	UPrimitiveComponent* Primitive = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	AActor* Actor = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	EDetectionType Type = EDetectionType::Interact;
};


USTRUCT(Blueprintable)
struct FPayloadCollision
{
	GENERATED_BODY();

	FPayloadCollision() {}
	FPayloadCollision(UPrimitiveComponent* _HitComp, UPrimitiveComponent* _OtherComp, FVector _Impact, FVector _HitLocation, FVector _Normal, TWeakObjectPtr<UPhysicalMaterial> _PhysMat)
		: HitComp(_HitComp), OtherComp(_OtherComp), Impact(_Impact), HitLocation(_HitLocation), Normal(_Normal), PhysMat(_PhysMat)
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	UPrimitiveComponent* HitComp = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	UPrimitiveComponent* OtherComp = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FVector Impact = FVector(0.f, 0.f, 0.f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FVector HitLocation = FVector(0.f, 0.f, 0.f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FVector Normal = FVector(0.f,0.f,1.f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	TWeakObjectPtr<UPhysicalMaterial> PhysMat;
};

USTRUCT(Blueprintable)
struct FPayloadSpline
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	UPrimitiveComponent* Primitive = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FSocketData Socket;
};


//Add Payload structs here
//...
//...
//...