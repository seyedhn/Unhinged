// Copyright 2024 Seyed Nasrollahi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "../Resource/ResourceSMC.h"
#include "../Resource/ResourceBox.h"
#include "../Resource/ResourceManager.h"
#include "../Damage/DamageManager.h"
#include "../Audio/AudioManager.h"
#include "../Heat/HeatManager.h"
#include "../Notification/NotificationManager.h"
#include "../Spline/PipeManager.h"
#include "../Sky/SkyManager.h"
#include "../UserSettings/UserSettingsActor.h"
#include "../Spline/SplineComp.h"
#include "../Postprocess/PostprocessManager.h"
#include "../Postprocess/FPCameraComp.h"
#include "../Storage/SpaceChargeSMC.h"
#include "../Equipment/EquipmentHandler.h"
#include "../Avatar/MovementComp.h"
#include "../Wormhole/SceneCaptureCubeComp.h"
#include "Engine/TextureRenderTargetCube.h"
#include "../Fuse/FusePDA.h"
#include "Sound/SoundMix.h"
#include "NiagaraSystem.h"
#include "ManagerBase.h"
#include "ManagerSettings.generated.h"


UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Manager"))
class THEGAME_API UManagerSettings : public UDeveloperSettings
{
	GENERATED_UCLASS_BODY()

public:

	//MANAGERS//

	UPROPERTY(config, EditAnywhere, Category = "Manager", meta = (BlueprintBaseOnly, DisplayName = "Damage Manager"))
	TSoftClassPtr<ADamageManager> DamageManager;

	UPROPERTY(config, EditAnywhere, Category = "Manager", meta = (BlueprintBaseOnly, DisplayName = "Postprocess Manager"))
	TSoftClassPtr<APostprocessManager> PostprocessManager;

	UPROPERTY(config, EditAnywhere, Category = "Manager", meta = (BlueprintBaseOnly, DisplayName = "Heat Manager"))
	TSoftClassPtr<AHeatManager> HeatManager;

	UPROPERTY(config, EditAnywhere, Category = "Manager", meta = (BlueprintBaseOnly, DisplayName = "Audio Manager"))
	TSoftClassPtr<AAudioManager> AudioManager;

	UPROPERTY(config, EditAnywhere, Category = "Manager", meta = (BlueprintBaseOnly, DisplayName = "Notification Manager"))
	TSoftClassPtr<ANotificationManager> NotificationManager;

	UPROPERTY(config, EditAnywhere, Category = "Manager", meta = (BlueprintBaseOnly, DisplayName = "Sky Manager"))
	TSoftClassPtr<ASkyManager> SkyManager;


	//USER SETTINGS
	UPROPERTY(config, EditAnywhere, Category = "User Settings", meta = (BlueprintBaseOnly, DisplayName = "User Settings Actor"))
	TSoftClassPtr<AUserSettingsActor> UserSettingsActor;

	//PAWNS
	UPROPERTY(config, EditAnywhere, Category = "Pawn", meta = (BlueprintBaseOnly, DisplayName = "Spectator Pawn"))
	TSoftClassPtr<ASpectatorPawn> SpectatorPawn;

	//RESOURCE
	UPROPERTY(config, EditAnywhere, Category = "Resource", meta = (BlueprintBaseOnly, DisplayName = "Resource SMC"))
	TSoftClassPtr<UResourceSMC> ResourceSMC;

	UPROPERTY(config, EditAnywhere, Category = "Resource", meta = (BlueprintBaseOnly, DisplayName = "Resource Box"))
	TSoftClassPtr<AResourceBox> ResourceBox;

	//FUSE//
	UPROPERTY(config, EditAnywhere, Category = "Fuse", meta = (DisplayName = "Fuse Sound"))
	TSoftObjectPtr<USoundBase> FuseSound;

	UPROPERTY(config, EditAnywhere, Category = "Fuse", meta = (DisplayName = "Fuse FX"))
	TSoftObjectPtr<UNiagaraSystem> FuseFX;


	//AVATAR//

	UPROPERTY(config, EditAnywhere, Category = "Avatar", meta = (BlueprintBaseOnly, DisplayName = "Equipment Handler"))
	TSoftClassPtr<UEquipmentHandler> EquipmentHandler;

	UPROPERTY(config, EditAnywhere, Category = "Avatar", meta = (BlueprintBaseOnly, DisplayName = "Movement Component"))
	TSoftClassPtr<UMovementComp> MovementComp;

	UPROPERTY(config, EditAnywhere, Category = "Avatar", meta = (BlueprintBaseOnly, DisplayName = "FP Camera Component"))
	TSoftClassPtr<UFPCameraComp> FPCameraComp;

	UPROPERTY(config, EditAnywhere, Category = "Avatar", meta = (BlueprintBaseOnly, DisplayName = "Illusion"))
	TSoftClassPtr<AActor> Illusion;

	//BLOCKS//

	UPROPERTY(config, EditAnywhere, Category = "Blocks", meta = (BlueprintBaseOnly, DisplayName = "Spline Component"))
	TSoftClassPtr<USplineComp> SplineComponent;

	UPROPERTY(config, EditAnywhere, Category = "Blocks", meta = (DisplayName = "Block Rewind Spline Mesh"))
	TSoftObjectPtr<UStaticMesh> BlockRewindStaticMesh;

	UPROPERTY(config, EditAnywhere, Category = "Blocks", meta = (DisplayName = "Block Charged Material"))
	TSoftObjectPtr<UMaterialInterface> BlockChargedMaterial;

	//GRID//
	UPROPERTY(config, EditAnywhere, Category = "Grid", meta = (DisplayName = "Grid Tether"))
	TSoftObjectPtr<UNiagaraSystem> Tether;

	//TIME BENDER//
	UPROPERTY(config, EditAnywhere, Category = "Block Rewind", meta = (DisplayName = "Rewind Sound"))
	TSoftObjectPtr<USoundBase> RewindSound;

	UPROPERTY(config, EditAnywhere, Category = "Block Rewind", meta = (DisplayName = "Rewind End Sound"))
	TSoftObjectPtr<USoundBase> RewindEndSound;

	UPROPERTY(config, EditAnywhere, Category = "Block Rewind", meta = (DisplayName = "Rewind Sound Mixer"))
	TSoftObjectPtr<USoundMix> RewindSoundMix;

	//Add Default Manager Classes here. Make sure they are of type TSoftClassPtr
	//...
	//...
	//...









	//~ UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return FName(TEXT("Game")); }
#if WITH_EDITOR
	virtual FText GetSectionText() const override { return NSLOCTEXT("TheGame", "ManagerSettingsSection", "Manager"); };
#endif

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};