// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Manager/ManagerBase.h"
#include "DamageManager.generated.h"



UENUM(BlueprintType)
enum class EDamageType : uint8
{
	None	 UMETA(DisplayName = "None"),
	Slash  UMETA(DisplayName = "Slash"),
	Smash  UMETA(DisplayName = "Smash"),
	Pierce UMETA(DisplayName = "Pierce"),
	Burn	 UMETA(DisplayName = "Burn"),
	Pure   UMETA(DisplayName = "Pure"),
	Toxic  UMETA(DisplayName = "Toxic"),
	Radiation UMETA(DisplayName = "Radiation"),
	Explosion UMETA(DisplayName = "Explosion"),
	Drown  UMETA(DisplayName = "Drown")

};


USTRUCT(Blueprintable)
struct FDamageStruct
{
	GENERATED_BODY();

	FDamageStruct()
	{};

	FDamageStruct(AActor* _HitActor, UPrimitiveComponent* _HitComp, float _Damage, EDamageType _DamageType, int32 _Index)
		: HitActor(_HitActor), HitComp(_HitComp), Damage(_Damage), DamageType(_DamageType), Index(_Index)
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	AActor* HitActor = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	UPrimitiveComponent* HitComp = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float Damage = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	EDamageType DamageType = EDamageType::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	int32 Index = 0;

};


USTRUCT(Blueprintable)
struct FHitDamageStruct
{
	GENERATED_BODY();

	FHitDamageStruct()
	{};

	FHitDamageStruct(AActor* _Actor, UPrimitiveComponent* _Comp, float _Damage, EDamageType _DamageType, int32 _Index, FVector _HitLocation, FVector _HitNormal, FName _HitBone)
		: Actor(_Actor), Comp(_Comp), Damage(_Damage), DamageType(_DamageType), Index(_Index), HitLocation(_HitLocation), HitNormal(_HitNormal), HitBone(_HitBone)
	{}


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	AActor* Actor = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	UPrimitiveComponent* Comp =nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float Damage = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	EDamageType DamageType = EDamageType::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	int32 Index = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FVector HitLocation = FVector(0.f);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FVector HitNormal = FVector(0.f);
		
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	FName HitBone;

};


USTRUCT(Blueprintable)
struct FRadialDamageStruct
{
	GENERATED_BODY();

	FRadialDamageStruct()
	{};

	FRadialDamageStruct(UPrimitiveComponent* _HitComp, float _Radius, float _Damage, float _Impulse, EDamageType _DamageType)
		: HitComp(_HitComp), Radius(_Radius), Damage(_Damage), Impulse(_Impulse), DamageType(_DamageType)
	{};


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	UPrimitiveComponent* HitComp = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float Radius = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float Damage = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	float Impulse = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Variables)
	EDamageType DamageType = EDamageType::None;

};





UCLASS(Blueprintable, BlueprintType)
class THEGAME_API ADamageManager : public AManagerBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DealHitDamage(FHitDamageStruct Damage); 

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ProcessRadialDamage(FRadialDamageStruct RadialDamage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ProcessOverlapDamage(FDamageStruct Damage);

};
