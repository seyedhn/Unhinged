#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"


struct THEGAME_API FItemTags : public FGameplayTagNativeAdder
{

public:

  //These are used in the old furnace system. can be eventually deleted
  FGameplayTag Ore;
  FGameplayTag Metal;
  FGameplayTag Biomass;
  FGameplayTag Fuel;


  FGameplayTag Food;
  FGameplayTag Equipment;
  FGameplayTag Craftable;
  FGameplayTag Upgrade;
  FGameplayTag Misc;

  FGameplayTag Block;
  FGameplayTag BlockStatic;
  FGameplayTag BlockDynamic;

  FGameplayTag Structure;
  FGameplayTag StructureSpline;
  FGameplayTag StructureStatic;

  FGameplayTag Foliage;
  FGameplayTag Plant;
  FGameplayTag SmallTree;
  FGameplayTag MediumTree;
  FGameplayTag LargeTree;
  FGameplayTag Rock;
  //Declare Tags here
  //...
  //...
  //...

  FORCEINLINE static const FItemTags& Get() { return ItemTags; }

protected:
  virtual void AddTags() override
  {
    UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();

    Ore = TagsManager.AddNativeGameplayTag(TEXT("Item.Ore"));
    Metal = TagsManager.AddNativeGameplayTag(TEXT("Item.Metal"));
    Biomass = TagsManager.AddNativeGameplayTag(TEXT("Item.Biomass"));
    Fuel = TagsManager.AddNativeGameplayTag(TEXT("Item.Fuel"));

    Food = TagsManager.AddNativeGameplayTag(TEXT("Item.Food"));
    Equipment = TagsManager.AddNativeGameplayTag(TEXT("Item.Equipment"));
    Craftable = TagsManager.AddNativeGameplayTag(TEXT("Item.Craftable"));
    Upgrade = TagsManager.AddNativeGameplayTag(TEXT("Item.Upgrade"));
    Misc = TagsManager.AddNativeGameplayTag(TEXT("Item.Misc"));

    Block = TagsManager.AddNativeGameplayTag(TEXT("Item.Block"));
    BlockStatic = TagsManager.AddNativeGameplayTag(TEXT("Item.Block.Static"));
    BlockDynamic = TagsManager.AddNativeGameplayTag(TEXT("Item.Block.Dynamic"));

    Structure = TagsManager.AddNativeGameplayTag(TEXT("Item.Structure"));
    StructureSpline = TagsManager.AddNativeGameplayTag(TEXT("Item.Structure.Spline"));
    StructureStatic = TagsManager.AddNativeGameplayTag(TEXT("Item.Structure.Static"));

    Foliage = TagsManager.AddNativeGameplayTag(TEXT("Item.Foliage"));
    Plant = TagsManager.AddNativeGameplayTag(TEXT("Item.Foliage.Plant"));
    SmallTree = TagsManager.AddNativeGameplayTag(TEXT("Item.Foliage.SmallTree"));
    MediumTree = TagsManager.AddNativeGameplayTag(TEXT("Item.Foliage.MediumTree"));
    LargeTree = TagsManager.AddNativeGameplayTag(TEXT("Item.Foliage.LargeTree"));
    Rock = TagsManager.AddNativeGameplayTag(TEXT("Item.Foliage.Rock"));
    //Add Tags here
    //...
    //...
    //...
  }

private:
  static FItemTags ItemTags;
};
