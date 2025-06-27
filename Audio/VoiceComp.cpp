#include "VoiceComp.h"
#include "Kismet/GameplayStatics.h"

void UVoiceComp::PlayDamageSound(EDamageType DamageType)
{
	if (DamageSounds->DamageSounds.Contains(DamageType))
	{
		USoundBase* Sound = DamageSounds->DamageSounds.Find(DamageType)->LoadSynchronous();
		UGameplayStatics::SpawnSound2D(this, Sound, 1.f, 1.f, 0.f, DamageConcurrency)->SetUISound(false);
	}

	if (DamageType != EDamageType::Drown)
	{
		UGameplayStatics::SpawnSound2D(this, DamageVO, 1.f, 1.f, 0.f, DamageVOConcurrency)->SetUISound(false);
	}
}

void UVoiceComp::OnVitalsUpdated(float HydrationValue, bool Dehydrated, float RadiationValue, bool Radiated, float SanityValue, bool Insane)
{
	if (!IsValid(Radiation))
	{
		if (Radiated)
		{
			Radiation = UGameplayStatics::SpawnSound2D(this, RadiationSound, 1.f, 1.f, 0.f, DamageConcurrency);
			Radiation->SetUISound(false);
		}
	}
	else
	{
		if (Radiated)
		{
			Radiation->SetFloatParameter(TEXT("RadiationAmt"), RadiationValue - 60);//60 needs to be replaced by Radiation Criticality
			return;
		}
		Radiation->Deactivate();
		Radiation->DestroyComponent();
	}
}

void UVoiceComp::OnStaminaUpdated(float Stamina)
{
	if (!Breathing && Stamina < 80.f)
	{
		Breathing = UGameplayStatics::SpawnSound2D(this, BreathingSound, 0.6f);
		Breathing->SetUISound(false);
		Breathing->OnAudioFinished.AddUniqueDynamic(this, &UVoiceComp::OnBreathingEnded);
		Breathing->SetFloatParameter(TEXT("Stamina"), Stamina*0.01);
		return;
	}

	if (Breathing)
	{
		if (Stamina < 80.f)	Breathing->SetFloatParameter(TEXT("Stamina"), Stamina * 0.01);
		else	Breathing->SetTriggerParameter(TEXT("Finished"));
	}


}

void UVoiceComp::OnBreathingEnded()
{
	if (Breathing->IsValidLowLevel())
	{
		Breathing->Deactivate();
		Breathing->DestroyComponent();
		Breathing = nullptr;
	}

}
