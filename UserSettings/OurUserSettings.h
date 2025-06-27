// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "SettingsPDA.h"
#include "../Tools/OurSystemLibrary.h"
#include "../Manager/ManagerSubsystem.h"
#include "OurUserSettings.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReloadSettingsDelegate);

UCLASS(Blueprintable)
class THEGAME_API UOurUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	UOurUserSettings();
	virtual void SetToDefaults() override;

	UFUNCTION(BlueprintCallable)
	void ResetToDefault(const FInstancedStruct& Setting);

	UFUNCTION(BlueprintCallable)
	static UOurUserSettings* GetOurUserSettings();

	UFUNCTION(BlueprintCallable)
	void InitialiseUserSettings(UWorld* World);


	UFUNCTION(BlueprintCallable, Category = "Settings", meta = (ReturnDisplayName = "IsDefaultValue"))
	bool ApplyBinarySetting(const FInstancedStruct& Setting, const bool Value);
	UFUNCTION(BlueprintCallable, Category = "Settings", meta = (ReturnDisplayName = "IsDefaultValue"))
	bool ApplySliderSetting(const FInstancedStruct& Setting, const float Value);
	UFUNCTION(BlueprintCallable, Category = "Settings", meta = (ReturnDisplayName = "IsDefaultValue"))
	bool ApplyMPSetting(const FInstancedStruct& Setting, const int32 Value);
	UFUNCTION(BlueprintCallable, Category = "Settings", meta = (ReturnDisplayName = "IsDefaultValue"))
	bool ApplyScalabilitySetting(const FInstancedStruct& Setting, const int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Settings", meta = (ReturnDisplayName = "IsDefaultValue"))
	bool GetBinarySetting(const FInstancedStruct& Setting, bool& Value);
	UFUNCTION(BlueprintCallable, Category = "Settings", meta = (ReturnDisplayName = "IsDefaultValue"))
	bool GetSliderSetting(const FInstancedStruct& Setting, float& Value);
	UFUNCTION(BlueprintCallable, Category = "Settings", meta = (ReturnDisplayName = "IsDefaultValue"))
	bool GetMPSetting(const FInstancedStruct& Setting, int32& Value);
	UFUNCTION(BlueprintCallable, Category = "Settings", meta = (ReturnDisplayName = "IsDefaultValue"))
	bool GetScalabilitySetting(const FInstancedStruct& Setting, int32& Value);



	//BLUEPRINT FUNCTIONS

	UFUNCTION(BlueprintCallable)
	UFPCameraComp* GetFPCamera();

	UFUNCTION(BlueprintCallable)
	APostProcessVolume* GetWorldPP();

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FReloadSettingsDelegate OnReloadSettings;


	private:

		void LoadSettingDataAssets();
		void SetDefaultValues();
		void ApplySavedSettings();
		void ApplySavedSetting(const FInstancedStruct& Setting);
		void ReloadSettings();

		//TYPE SETTERS//
		void SetBinary(const FName Name, const bool Value);
		void SetMP(const FName Name, const int32 Value);
		void SetScalability(const FName Name, const int32 Value);
		void SetSlider(const FName Name, const float Value);

		//TYPE GETTERS//
		bool GetBinary(const FName Name);
		int32 GetMP(const FName Name);
		float GetSlider(const FName Name);

		//HELPER FUNCTIONS
		void ExecuteConsoleCommand(const FString& CVar, const FString& Value);
		FName Setter(const FName Name) const;
		FName Getter(const FName Name) const;


		UPROPERTY()
		TArray<USettingsPDA*> SettingsDAs;
		UPROPERTY()
		UWorld* World;
		UPROPERTY()
		APostProcessVolume* WorldPP;
		UPROPERTY()
		UFPCameraComp* FPCamera;
		UPROPERTY()
		AUserSettingsActor* UserSettingsActor;



protected:

	//SETTINGS IMPLEMENTATIONS//

	//GRAPHICS//

	UFUNCTION()
	void SetLumen(const bool Value);
	UFUNCTION()
	void SetMotionBlur(const bool Value);
	UFUNCTION()
	void SetNaniteVSM(const bool Value);
	UFUNCTION()
	void SetAAMethod(const int32 Value);
	UFUNCTION()
	void SetVolumetricCloud(const bool Value);
	UFUNCTION()
	void SetDFS(const bool Value);
	UFUNCTION()
	void SetMSAAQuality(const int32 Value);
	UFUNCTION()
	void SetSSAAQuality(const int32 Value);
	UFUNCTION()
	void SetVolumetricFogQuality(const int32 Value);
	UFUNCTION()
	void SetRHI(const int32 Value);


	//VIDEO//

	UFUNCTION()
	void SetGamma(const float Value);
	UFUNCTION()
	void SetScreenMode(const int32 Value);
	UFUNCTION()
	int32 GetScreenMode();
	UFUNCTION()
	void SetResolution(const int32 Value);
	UFUNCTION()
	int32 GetResolution();
	UFUNCTION()
	void SetScreenMessages(const bool Value);


	//CAMERA//

	UFUNCTION()
	void SetFOV(const float Value);
	UFUNCTION()
	void SetHeadBobbing(const bool Value);
	UFUNCTION()
	void SetFisheye(const bool Value);

	//AUDIO

	//SKY//

	UFUNCTION()
	void SetSkyPreset(const int32 Value);

	//POSTPROCESS
	UFUNCTION()
	void SetPostProcess(const bool Value);
	UFUNCTION()
	void SetBlendWeight(const float Value);
	UFUNCTION()
	void SetColorGrading(const bool Value);
	UFUNCTION()
	void SetBloomMethod(const int32 Value);
	UFUNCTION()
	void SetBloomIntensity(const float Value);
	UFUNCTION()
	void SetBloomThreshold(const float Value);
	UFUNCTION()
	void SetAOIntensity(const float Value);
	UFUNCTION()
	void SetAORadius(const float Value);
	UFUNCTION()
	void SetChromatic(const float Value);
	UFUNCTION()
	void SetDirtMask(const float Value);
	UFUNCTION()
	void SetVignette(const float Value);
	UFUNCTION()
	void SetGradingTemp(const float Value);
	UFUNCTION()
	void SetGradingTint(const float Value);
	UFUNCTION()
	void SetFilmSlope(const float Value);
	UFUNCTION()
	void SetToneCurve(const float Value);
	UFUNCTION()
	void SetFilmGrain(const float Value);
	UFUNCTION()
	void SetFilmGrainShadows(const float Value);



public:

	//GRAPHICS//
	UPROPERTY(Config)
	bool NaniteVSM;
	UPROPERTY(Config)
	bool Lumen;
	UPROPERTY(Config)
	bool MotionBlur;
	UPROPERTY(Config, BlueprintReadOnly)
	int32 AAMethod;
	UPROPERTY(Config)
	int32 MSAAQuality;
	UPROPERTY(Config)
	int32 SSAAQuality;
	UPROPERTY(Config)
	int32 VolumetricFogQuality;
	UPROPERTY(Config)
	int32 RHI;
	UPROPERTY(Config)
	bool VolumetricCloud;
	UPROPERTY(Config)
	bool DFS;
	UPROPERTY(Config, BlueprintReadOnly)
	int32 NumShadowCascades;
	UPROPERTY(Config, BlueprintReadOnly)
	float DynamicShadowDistance;

	//VIDEO//

	UPROPERTY(Config)
	float Gamma;
	UPROPERTY(Config, BlueprintReadOnly)
	bool PauseBG;
	UPROPERTY(Config, BlueprintReadOnly)
	bool PauseGame;
	UPROPERTY(Config)
	bool ScreenMessages;

	//CAMERA//
	UPROPERTY(Config, BlueprintReadOnly)
	float FOV;
	UPROPERTY(Config)
	bool HeadBobbing;
	UPROPERTY(Config, BlueprintReadOnly)
	bool Fisheye;


	//POSTPROCESS//
	UPROPERTY(Config)
	bool PostProcess;
	UPROPERTY(Config)
	float BlendWeight;
	UPROPERTY(Config)
	bool ColorGrading;
	UPROPERTY(Config)
	int32 BloomMethod;
	UPROPERTY(Config)
	float BloomIntensity;
	UPROPERTY(Config)
	float BloomThreshold;
	UPROPERTY(Config)
	float AOIntensity;
	UPROPERTY(Config)
	float AORadius;
	UPROPERTY(Config)
	float Chromatic;
	UPROPERTY(Config)
	float DirtMask;
	UPROPERTY(Config)
	float Vignette;
	UPROPERTY(Config)
	float GradingTemp;
	UPROPERTY(Config)
	float GradingTint;
	UPROPERTY(Config)
	float FilmSlope;
	UPROPERTY(Config)
	float ToneCurve;
	UPROPERTY(Config)
	float FilmGrain;
	UPROPERTY(Config)
	float FilmGrainShadows;

	//Audio//
	UPROPERTY(Config)
	float MasterVolume;
	UPROPERTY(Config)
	float AmbienceVolume;
	UPROPERTY(Config)
	float VoiceVolume;














	/*
	UFUNCTION(BlueprintCallable)
	void SetSoundVolume(class USoundMix* InSoundMixModifier, class USoundClass* InSoundClass, float Volume);

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "WidgetClass"))
	UUserWidget* GetWidgetOfClass(TSubclassOf<UUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "ActorClass"))
	AActor* GetActorOfClass(TSubclassOf<AActor> ActorClass);

	UFUNCTION(BlueprintCallable)
	void SetGamePaused(bool Pause);
	*/





};
