// Copyright 2024 Seyed Nasrollahi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OurSystemLibrary.generated.h"

USTRUCT()
struct FFunctionParamBool
{
	GENERATED_BODY()

	FFunctionParamBool() {}
	FFunctionParamBool(bool _param)
		: Param(_param) {
	}
	bool Param = false;
};

USTRUCT()
struct FFunctionParamInt
{
	GENERATED_BODY()

	FFunctionParamInt() {}
	FFunctionParamInt(int32 _param)
		: Param(_param) {
	}
	int32 Param = 0;
};

USTRUCT()
struct FFunctionParamFloat
{
	GENERATED_BODY()

	FFunctionParamFloat() {}
	FFunctionParamFloat(float _param)
		: Param(_param) {
	}
	float Param = 0.f;
};


UCLASS(Blueprintable)
class THEGAME_API UOurSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:

	static TArray<FAssetData> FindDataAssetsOfClass(UClass* _type, bool _bRecursivePaths = true, bool _bRecursiveClasses = true, const FName DataAssetPath = TEXT("/Game"), bool _bScan = false);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static bool DoesHitImplementInterface(const FHitResult& Hit, TSubclassOf<UInterface> Interface, UObject*& Object);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static bool DoesObjectImplementInterface(AActor* Actor, UPrimitiveComponent* Comp, TSubclassOf<UInterface> Interface, UObject*& Object);


	//VARIABLE SETTERS

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void SetPropertyFloat(UObject* Target, FName PropertyName, float PropertyValue);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void SetPropertyFloatNative(UObject* Target, FName PropertyName, float PropertyValue);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void SetPropertyInt(UObject* Target, FName PropertyName, int32 PropertyValue);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void SetPropertyBool(UObject* Target, FName PropertyName, bool PropertyValue);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void SetPropertyByte(UObject* Target, FName PropertyName, uint8 PropertyValue);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void SetPropertyObject(UObject* Target, FName PropertyName, UObject* PropertyValue);
		
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void SetPropertyColor(UObject* Target, FName PropertyName, FLinearColor PropertyValue);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void SetPropertyRotator(UObject* Target, FName PropertyName, FRotator PropertyValue);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void SetPropertyVector2D(UObject* Target, FName PropertyName, FVector2D PropertyValue);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void SetPropertyVector(UObject* Target, FName PropertyName, FVector PropertyValue);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void SetPropertySoftObject(UObject* Target, FName PropertyName, TSoftObjectPtr<UObject> PropertyValue);

	//VARIABLE GETTERS//

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static bool GetPropertyBool(UObject* Target, FName PropertyName);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static int32 GetPropertyInt(UObject* Target, FName PropertyName);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static float GetPropertyFloat(UObject* Target, FName PropertyName);

	//HAS PROPERTY//
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static bool HasPropertyBool(UObject* Target, FName PropertyName);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static int32 HasPropertyInt(UObject* Target, FName PropertyName);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static float HasPropertyFloat(UObject* Target, FName PropertyName);

	//HAS FUNCTION//

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static bool HasFunction(UObject* Target, const FName FunctionName);

	//EXECUTE FUNCTIONS//

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void ExecuteFunctionVoid(UObject* Target, const FName FunctionName);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void ExecuteFunctionBool(UObject* Target, const FName FunctionName, const bool Value);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void ExecuteFunctionInt(UObject* Target, const FName FunctionName, const int32 Value);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void ExecuteFunctionFloat(UObject* Target, const FName FunctionName, const float Value);



	//RETURN FUNCTIONS//


	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static bool ReturnFunctionBool(UObject* Target, const FName FunctionName);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static int32 ReturnFunctionInt(UObject* Target, const FName FunctionName);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static float ReturnFunctionFloat(UObject* Target, const FName FunctionName);









	template<typename T, typename Type>
	static void SetProperty(UObject* Target, FName PropertyName, const T& PropertyValue)
	{
		if (Type* Property = FindFProperty<Type>(Target->GetClass(), PropertyName))
		{
			Property->SetPropertyValue_InContainer(Target, PropertyValue);
		}
	}

	template<typename T>
	static void SetStructProperty(UObject* Target, FName PropertyName, const T& PropertyValue)
	{
		if (FProperty* Property = Target->GetClass()->FindPropertyByName(PropertyName))
		{
			if (const FStructProperty* StructProperty = CastField<FStructProperty>(Property))
			{
				if (StructProperty->Struct == TBaseStructure<T>::Get())
				{
					void* address = Property->ContainerPtrToValuePtr<void>(Target);
					StructProperty->CopyCompleteValue(address, &PropertyValue);
				}
			}
		}
	}

	template<typename Type>
	static bool HasProperty(UObject* Target, FName PropertyName)
	{
		if (Type* Property = FindFProperty<Type>(Target->GetClass(), PropertyName))
		{
			return true;
		}
		return false;
	}

	template<typename T, typename Type>
	static T GetProperty(UObject* Target, FName PropertyName)
	{
		if (Type* Property = FindFProperty<Type>(Target->GetClass(), PropertyName))
		{
			return Property->GetPropertyValue_InContainer(Target);
		}
		return T();
	}

	template<typename T, typename Type>
	static void ExecuteFunction(UObject* Target, const FName& FunctionName, const T Value)
	{
		UFunction* Func = Target->GetClass()->FindFunctionByName(FunctionName);
		if (IsValid(Func))
		{
			Type Param = Type(Value);
			Target->ProcessEvent(Func, &Param);
		}
	}

	template<typename Type>
	static Type ReturnFunction(UObject* Target, const FName& FunctionName)
	{
		UFunction* Func = Target->GetClass()->FindFunctionByName(FunctionName);
		Type Param;
		if (IsValid(Func))
		{
			Target->ProcessEvent(Func, &Param);
		}
		return Param;
	}


};








/* ANOTHER METHOD THAT USES CASTFIELD
if (FProperty* Property = Object->GetClass()->FindPropertyByName(PropertyName))
{
	if(const Type* TypedProperty = CastField<Type>(Property))
	{
		void* address = TypedProperty->ContainerPtrToValuePtr<void>(Object);
		TypedProperty->SetPropertyValue(address, PropertyValue);
		//TPropertyTypeFundamentals<T>::SetPropertyValue(address, PropertyValue);
	}
}
*/