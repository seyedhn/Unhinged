#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
//#include "ChannelTags.generated.h"


struct THEGAME_API FChannelTags : public FGameplayTagNativeAdder
{

public:

  FGameplayTag Hint;
  FGameplayTag VoiceOver;
  FGameplayTag Hallucination;
  FGameplayTag TitleCard;
  FGameplayTag Sky;
  FGameplayTag Objective;

  FGameplayTag ItemDropped;
  FGameplayTag ItemConsumed;
  FGameplayTag ItemCollected;
  FGameplayTag ItemSpawned;
  FGameplayTag InventoryUpdated;
  FGameplayTag BlockUnlocked;
  FGameplayTag Interactions;
  FGameplayTag ItemDetected;
  FGameplayTag UpgradePickup;
  FGameplayTag UpgradeInstalled;
  FGameplayTag EquipmentPickup;
  FGameplayTag Equip;
  FGameplayTag Notification;
  FGameplayTag Warning;
  FGameplayTag GameSaved;
  FGameplayTag GameLoaded;
  FGameplayTag GamePaused;
  FGameplayTag PPActivateMaterial;
  FGameplayTag PPActivateSettings;
  FGameplayTag PPExposure;
  FGameplayTag StorageNetwork;
  FGameplayTag HyperMail;
  FGameplayTag Trial;
  FGameplayTag Shelter;
  FGameplayTag Sleep;
  FGameplayTag TierUp;
  FGameplayTag Craft;
  FGameplayTag GizmoClosed;
  FGameplayTag PipeNetwork;
  FGameplayTag PipeStartFlow;
  FGameplayTag Collision;
  FGameplayTag WormholeCapture;
  FGameplayTag ReleaseObject;
  FGameplayTag GrabObject;
  //Declare Tags here
  //...
  //...
  //...

  FORCEINLINE static const FChannelTags& Get() { return ChannelTags; }

protected:
  virtual void AddTags() override
  {
    UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();

    //TRIGGERS//
    Hint = TagsManager.AddNativeGameplayTag(TEXT("Trigger.Hint"));
    VoiceOver = TagsManager.AddNativeGameplayTag(TEXT("Trigger.VoiceOver"));
    Hallucination = TagsManager.AddNativeGameplayTag(TEXT("Trigger.Hallucination"));
    TitleCard = TagsManager.AddNativeGameplayTag(TEXT("Trigger.TitleCard"));
    Sky = TagsManager.AddNativeGameplayTag(TEXT("Trigger.Sky"));
    Objective = TagsManager.AddNativeGameplayTag(TEXT("Trigger.Objective"));

    //EVENTS//
    ItemSpawned = TagsManager.AddNativeGameplayTag(TEXT("Event.Item.Spawned"));
    ItemCollected = TagsManager.AddNativeGameplayTag(TEXT("Event.Item.Collected"));
    ItemDetected = TagsManager.AddNativeGameplayTag(TEXT("Event.Item.Detected"));
    ItemDropped = TagsManager.AddNativeGameplayTag(TEXT("Event.Item.Dropped"));
    ItemConsumed = TagsManager.AddNativeGameplayTag(TEXT("Event.Item.Consumed"));

    PPActivateMaterial = TagsManager.AddNativeGameplayTag(TEXT("Event.PP.ActivateMaterial"));
    PPActivateSettings = TagsManager.AddNativeGameplayTag(TEXT("Event.PP.ActivateSettings"));
    PPExposure = TagsManager.AddNativeGameplayTag(TEXT("Event.PP.Exposure"));

    GameSaved = TagsManager.AddNativeGameplayTag(TEXT("Event.Game.Saved"));
    GameLoaded = TagsManager.AddNativeGameplayTag(TEXT("Event.Game.Loaded"));
    GamePaused = TagsManager.AddNativeGameplayTag(TEXT("Event.Game.Paused"));

    Interactions = TagsManager.AddNativeGameplayTag(TEXT("Event.Interactions"));
    InventoryUpdated = TagsManager.AddNativeGameplayTag(TEXT("Event.InventoryUpdated"));
    BlockUnlocked = TagsManager.AddNativeGameplayTag(TEXT("Event.BlockUnlocked"));
    UpgradePickup = TagsManager.AddNativeGameplayTag(TEXT("Event.Upgrade.Pickup"));
    UpgradeInstalled = TagsManager.AddNativeGameplayTag(TEXT("Event.Upgrade.Install"));
    EquipmentPickup = TagsManager.AddNativeGameplayTag(TEXT("Event.NewWeaponPickedUp"));
    Equip = TagsManager.AddNativeGameplayTag(TEXT("Event.Equip"));
    Notification = TagsManager.AddNativeGameplayTag(TEXT("Event.Notification"));
    Warning = TagsManager.AddNativeGameplayTag(TEXT("Event.Warning"));
    StorageNetwork = TagsManager.AddNativeGameplayTag(TEXT("Event.Storage.Network"));
    HyperMail = TagsManager.AddNativeGameplayTag(TEXT("Event.HyperMail"));
    Craft = TagsManager.AddNativeGameplayTag(TEXT("Event.Craft"));
    Trial = TagsManager.AddNativeGameplayTag(TEXT("Event.Trial"));
    Shelter = TagsManager.AddNativeGameplayTag(TEXT("Event.Shelter"));
    Sleep = TagsManager.AddNativeGameplayTag(TEXT("Event.Sleep"));
    TierUp = TagsManager.AddNativeGameplayTag(TEXT("Event.TierUp"));
    GizmoClosed = TagsManager.AddNativeGameplayTag(TEXT("Event.GizmoClosed"));
    PipeNetwork = TagsManager.AddNativeGameplayTag(TEXT("Event.Pipe.Network"));
    PipeStartFlow = TagsManager.AddNativeGameplayTag(TEXT("Event.Pipe.StartFlow"));
    Collision = TagsManager.AddNativeGameplayTag(TEXT("Event.Collision"));
    WormholeCapture = TagsManager.AddNativeGameplayTag(TEXT("Event.WormholeCapture"));
    ReleaseObject = TagsManager.AddNativeGameplayTag(TEXT("Event.Object.Released"));
    GrabObject = TagsManager.AddNativeGameplayTag(TEXT("Event.Object.Grabbed"));



    //Add Tags here
    //...
    //...
    //...
  }

private:
  static FChannelTags ChannelTags;
};
