// Copyright 2024 Seyed Nasrollahi. All Rights Reserved.

#include "OurSystemLibrary.h"
#include "Engine/AssetManager.h"
#include "AssetRegistry/AssetData.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/KismetSystemLibrary.h"


TArray<FAssetData> UOurSystemLibrary::FindDataAssetsOfClass(UClass* _type, bool _bRecursivePaths, bool _bRecursiveClasses, const FName DataAssetPath, bool bScan)
{
  if (bScan)
  {
    UAssetManager::Get().ScanPathForPrimaryAssets(FPrimaryAssetType(_type->GetFName()), DataAssetPath.ToString(), _type, false);
  }

  FAssetRegistryModule& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

  FARFilter Filter;
  Filter.bRecursivePaths = _bRecursivePaths;
  Filter.bRecursiveClasses = _bRecursiveClasses;
  Filter.ClassPaths.Add(_type->GetClassPathName());
  Filter.PackagePaths.Add(DataAssetPath);

  TArray<FAssetData> AssetData;
  AssetRegistry.Get().GetAssets(Filter, AssetData);

  return AssetData;
}

bool UOurSystemLibrary::DoesHitImplementInterface(const FHitResult& Hit, TSubclassOf<UInterface> Interface, UObject*& Object)
{
  return DoesObjectImplementInterface(Hit.GetActor(), Hit.Component.Get(), Interface, Object);
}

bool UOurSystemLibrary::DoesObjectImplementInterface(AActor* Actor, UPrimitiveComponent* Comp, TSubclassOf<UInterface> Interface, UObject*& Object)
{
  
  if (UKismetSystemLibrary::DoesImplementInterface(Comp, Interface))
  {
    Object = Comp;
    return true;
  }

  if (UKismetSystemLibrary::DoesImplementInterface(Actor, Interface))
  {
    Object = Actor;
    return true;
  }

  Object = nullptr;
  return false;
}



void UOurSystemLibrary::SetPropertyFloat(UObject* Target, FName PropertyName, float PropertyValue)
{
  SetProperty<float, FDoubleProperty>(Target, PropertyName, PropertyValue);
}

void UOurSystemLibrary::SetPropertyFloatNative(UObject* Target, FName PropertyName, float PropertyValue)
{
  SetProperty<float, FFloatProperty>(Target, PropertyName, PropertyValue);
}

void UOurSystemLibrary::SetPropertyInt(UObject* Target, FName PropertyName, int32 PropertyValue)
{
  SetProperty<int32, FIntProperty>(Target, PropertyName, PropertyValue);
}

void UOurSystemLibrary::SetPropertyObject(UObject* Target, FName PropertyName, UObject* PropertyValue)
{
  SetProperty<UObject*, FObjectProperty>(Target, PropertyName, PropertyValue);
}

void UOurSystemLibrary::SetPropertyBool(UObject* Target, FName PropertyName, bool PropertyValue)
{
  SetProperty<bool, FBoolProperty>(Target, PropertyName, PropertyValue);
}

void UOurSystemLibrary::SetPropertyByte(UObject* Target, FName PropertyName, uint8 PropertyValue)
{
  SetProperty<uint8, FByteProperty>(Target, PropertyName, PropertyValue);
}

void UOurSystemLibrary::SetPropertyColor(UObject* Target, FName PropertyName, FLinearColor PropertyValue)
{
  SetStructProperty<FLinearColor>(Target, PropertyName, PropertyValue);
}

void UOurSystemLibrary::SetPropertyRotator(UObject* Target, FName PropertyName, FRotator PropertyValue)
{
  SetStructProperty<FRotator>(Target, PropertyName, PropertyValue);
}

void UOurSystemLibrary::SetPropertyVector2D(UObject* Target, FName PropertyName, FVector2D PropertyValue)
{
  SetStructProperty<FVector2D>(Target, PropertyName, PropertyValue);
}

void UOurSystemLibrary::SetPropertyVector(UObject* Target, FName PropertyName, FVector PropertyValue)
{
  SetStructProperty<FVector>(Target, PropertyName, PropertyValue);
}

void UOurSystemLibrary::SetPropertySoftObject(UObject* Target, FName PropertyName, TSoftObjectPtr<UObject> PropertyValue)
{
  if (FSoftObjectProperty* Property = FindFProperty<FSoftObjectProperty>(Target->GetClass(), PropertyName))
  {
    Property->SetPropertyValue_InContainer(Target, FSoftObjectPtr(PropertyValue.ToSoftObjectPath()));
  }
}


//VARIABLE GETTERS//

bool UOurSystemLibrary::GetPropertyBool(UObject* Target, FName PropertyName)
{
  return GetProperty<bool, FBoolProperty>(Target, PropertyName);
}

int32 UOurSystemLibrary::GetPropertyInt(UObject* Target, FName PropertyName)
{
  return GetProperty<int32, FIntProperty>(Target, PropertyName);
}

float UOurSystemLibrary::GetPropertyFloat(UObject* Target, FName PropertyName)
{
  return GetProperty<float, FFloatProperty>(Target, PropertyName);
}

//HAS PROPERTY//

bool UOurSystemLibrary::HasPropertyBool(UObject* Target, FName PropertyName)
{
  return HasProperty<FBoolProperty>(Target, PropertyName);
}

int32 UOurSystemLibrary::HasPropertyInt(UObject* Target, FName PropertyName)
{
  return HasProperty<FIntProperty>(Target, PropertyName);
}

float UOurSystemLibrary::HasPropertyFloat(UObject* Target, FName PropertyName)
{
  return HasProperty<FFloatProperty>(Target, PropertyName);
}


//HAS FUNCTION//

bool UOurSystemLibrary::HasFunction(UObject* Target, const FName FunctionName)
{
  return IsValid(Target->GetClass()->FindFunctionByName(FunctionName));
}


//EXECUTE FUNCTIONS//

void UOurSystemLibrary::ExecuteFunctionVoid(UObject* Target, const FName FunctionName)
{
  UFunction* Func = Target->GetClass()->FindFunctionByName(FunctionName);
  if (IsValid(Func))
  {
    Target->ProcessEvent(Func, nullptr);
  }
}

void UOurSystemLibrary::ExecuteFunctionBool(UObject* Target, const FName FunctionName, const bool Value)
{
  ExecuteFunction<bool, FFunctionParamBool>(Target, FunctionName, Value);
}

void UOurSystemLibrary::ExecuteFunctionInt(UObject* Target, const FName FunctionName, const int32 Value)
{
  ExecuteFunction<int32, FFunctionParamInt>(Target, FunctionName, Value);
}

void UOurSystemLibrary::ExecuteFunctionFloat(UObject* Target, const FName FunctionName, const float Value)
{
  ExecuteFunction<float, FFunctionParamFloat>(Target, FunctionName, Value);
}


//RETURN FUNCTIONS//

bool UOurSystemLibrary::ReturnFunctionBool(UObject* Target, const FName FunctionName)
{
  return ReturnFunction<FFunctionParamBool>(Target, FunctionName).Param;
}

int32 UOurSystemLibrary::ReturnFunctionInt(UObject* Target, const FName FunctionName)
{
  return ReturnFunction<FFunctionParamInt>(Target, FunctionName).Param;
}

float UOurSystemLibrary::ReturnFunctionFloat(UObject* Target, const FName FunctionName)
{
  return ReturnFunction<FFunctionParamFloat>(Target, FunctionName).Param;
}
