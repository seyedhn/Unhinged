                                                                                                                                                                                                                                                                                                                                                                 #include "AudioManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "AudioDevice.h"

void AAudioManager::BeginPlay()
{

	Super::BeginPlay();

  UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);

  MessageSubsystem.RegisterListener(FChannelTags::Get().Collision, this, &AAudioManager::PlayCollisionSound);

}



void AAudioManager::PlayCollisionSound_Implementation(FGameplayTag Channel, const FPayloadCollision& Payload)
{
  UPrimitiveComponent* HitComp = Payload.HitComp;

  //This checks for head-on collision and not slide
  if (UKismetMathLibrary::Abs(HitComp->GetComponentVelocity().Dot(Payload.Normal)) > 3.f)
  {

    if (USoundBase* CollisionSound = CollisionSounds->GetCollisionSound(GetPhysMatOfPrimitive(HitComp), Payload.PhysMat.Get()).LoadSynchronous())
    {    
    float CompVelSq = HitComp->GetComponentVelocity().SquaredLength();
    float VolMult = FMath::Clamp(CompVelSq / 250000, 0.2f, 1.f);
    float PitchMult = FMath::Clamp(CompVelSq / 400000, 0.9f, 1.f);
    

    if (CompVelSq > 2000)
    {
        UGameplayStatics::SpawnSoundAtLocation(this, CollisionSound, Payload.HitLocation,
            UE::Math::TRotator<double>::ZeroRotator,
            VolMult, PitchMult, 0.f,
            CollisionAttenuation, CollisionConcurrency
            , true);
    }
    }

  }
    
}

void AAudioManager::PlayFrictionSound(UPrimitiveComponent* HitComp, UPhysicalMaterial* PhysMat, float Speed)
{
	if(IsValid(PhysMat))
	{
  	FFrictionSound friction(HitComp, PhysMat->SurfaceType);
		if (ActiveFrictionSounds.Contains(friction))
		{
			UFrictionSoundComp* AudioComp = ActiveFrictionSounds.FindRef(friction);
			 //Added this check because was getting occasional crashes here due to FrictionSound not being valid. Needs proper diognosis.
			if (IsValid(AudioComp) && IsValid(AudioComp->GetOwner()))
			{
				AudioComp->CollisionDetected(Speed);
				return;
			}
			
			
		}

		if (USoundBase* FrictionSound = CollisionSounds->GetFrictionSound(PhysMat).LoadSynchronous())
		{
			UFrictionSoundComp* FrictionSoundComp = SpawnCollisionSound(FrictionSound, HitComp, NAME_None,
				FVector(ForceInit), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset,
				true, 1.f, 1.f, 0.f, CollisionAttenuation, FrictionConcurrency, true);

			//friction.FrictionSound = FrictionSoundComp;
			ActiveFrictionSounds.Add(friction, FrictionSoundComp);
			FrictionSoundComp->Initialise(this, friction);
		}
	}


}

void AAudioManager::TerminateFrictionSound(const FFrictionSound& FrictionSound)
{
	ActiveFrictionSounds.Remove(FrictionSound);
}



UPhysicalMaterial* AAudioManager::GetPhysMatOfPrimitive(UPrimitiveComponent* HitComp) const
{
  if (UPhysicalMaterial* PhysMat = HitComp->GetMaterial(0)->GetPhysicalMaterial())
  {
    return PhysMat;
  }
  else
  {
    return HitComp->GetBodyInstance()->GetSimplePhysicalMaterial();
  }
}





UFrictionSoundComp* AAudioManager::SpawnCollisionSound(USoundBase* Sound, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation,
	EAttachLocation::Type LocationType, bool bStopWhenAttachedToDestroyed, float VolumeMultiplier, float PitchMultiplier, float StartTime, USoundAttenuation* AttenuationSettings,
	USoundConcurrency* ConcurrencySettings, bool bAutoDestroy)
{
	
	if (!Sound)
	{
		return nullptr;
	}

	if (!AttachToComponent)
	{
		UE_LOG(LogScript, Warning, TEXT("AudioManager::SpawnCollisionSound: NULL AttachComponent specified! Trying to spawn sound [%s],"), *Sound->GetName());
		return nullptr;
	}

	UWorld* const ThisWorld = AttachToComponent->GetWorld();
	if (ThisWorld && ThisWorld->IsNetMode(NM_DedicatedServer))
	{
		// FAudioDevice::CreateComponent will fail to create the AudioComponent in a real dedicated server, but we need to check netmode here for Editor support.
		return nullptr;
	}

	// Location used to check whether to create a component if out of range
	FVector TestLocation = Location;
	if (LocationType == EAttachLocation::KeepRelativeOffset)
	{
		if (AttachPointName != NAME_None)
		{
			TestLocation = AttachToComponent->GetSocketTransform(AttachPointName).TransformPosition(Location);
		}
		else
		{
			TestLocation = AttachToComponent->GetComponentTransform().TransformPosition(Location);
		}
	}
	else if (LocationType == EAttachLocation::SnapToTarget || LocationType == EAttachLocation::SnapToTargetIncludingScale)
	{
		// If AttachPointName is NAME_None, will return just the component position
		TestLocation = AttachToComponent->GetSocketLocation(AttachPointName);
	}

	FAudioDevice::FCreateComponentParams Params(ThisWorld, AttachToComponent->GetOwner());
	Params.SetLocation(TestLocation);
	Params.bStopWhenOwnerDestroyed = bStopWhenAttachedToDestroyed;
	Params.AttenuationSettings = AttenuationSettings;
	Params.AudioComponentClass = UFrictionSoundComp::StaticClass(); //THIS IS EXTRA ADDED HERE SO WE MAKE OUR OWN CLASS

	if (ConcurrencySettings)
	{
		Params.ConcurrencySet.Add(ConcurrencySettings);
	}

	UAudioComponent* AudioComp = FAudioDevice::CreateComponent(Sound, Params);
	UFrictionSoundComp* AudioComponent = Cast<UFrictionSoundComp>(AudioComp);
	if (AudioComponent)
	{
		if (UWorld* ComponentWorld = AudioComponent->GetWorld())
		{
			const bool bIsInGameWorld = ComponentWorld->IsGameWorld();

			if (LocationType == EAttachLocation::SnapToTarget || LocationType == EAttachLocation::SnapToTargetIncludingScale)
			{
				AudioComponent->AttachToComponent(AttachToComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachPointName);
			}
			else
			{
				AudioComponent->AttachToComponent(AttachToComponent, FAttachmentTransformRules::KeepRelativeTransform, AttachPointName);
				if (LocationType == EAttachLocation::KeepWorldPosition)
				{
					AudioComponent->SetWorldLocationAndRotation(Location, Rotation);
				}
				else
				{
					AudioComponent->SetRelativeLocationAndRotation(Location, Rotation);
				}
			}

			AudioComponent->SetVolumeMultiplier(VolumeMultiplier);
			AudioComponent->SetPitchMultiplier(PitchMultiplier);
			AudioComponent->bAllowSpatialization = Params.ShouldUseAttenuation();
			AudioComponent->bIsUISound = !bIsInGameWorld;
			AudioComponent->bAutoDestroy = bAutoDestroy;
			AudioComponent->SubtitlePriority = Sound->GetSubtitlePriority();
			AudioComponent->Play(StartTime);
		}
	}

	return AudioComponent;
}