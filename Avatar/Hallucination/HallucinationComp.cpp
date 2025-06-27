#include "HallucinationComp.h"
#include "../Avatar.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "../../Manager/ManagerSettings.h"


//SANITY-BASED HALLUCINATIONS

void UHallucinationComp::ActivateInsanity(bool Insanity)
{
    if (Insanity)
    {
        GetWorld()->GetTimerManager().SetTimer(InsanityTimer, this, &UHallucinationComp::RandomlyPerformInsanity, 5.0, true);
       // Cast<AAvatar>(GetOwner())->VitalsComp->OnVitalsUpdated
        return;
    }

    GetWorld()->GetTimerManager().ClearTimer(InsanityTimer);
    UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().Sky, FPayloadKey(TEXT("HyperMail"))); //Edit this to the default

}

void UHallucinationComp::RandomlyPerformInsanity()
{
    float SkipChance = (Cast<AAvatar>(GetOwner())->VitalsComp->Sanity)*0.01;
    if (UKismetMathLibrary::RandomBoolWithWeight(SkipChance))
    {
        return;
    }

    int32 Type = UKismetMathLibrary::RandomIntegerInRange(0, 2);

    switch (Type)
    {
    case 0:
        Hallucinate();
        break;
    case 1:
        SeeIllusion();
        break;
    case 2:
        SkyChange();
        break;
    }
}

void UHallucinationComp::Hallucinate()
{
    TArray<FHallucination> values;
    DA->Hallucinations.GenerateValueArray(values);
    FHallucination value = values[UKismetMathLibrary::RandomIntegerInRange(0, values.Num()-1)];
    Cast<AAvatar>(GetOwner())->AvatarUI->PlayHallucination(value);   
}

void UHallucinationComp::SeeIllusion()
{
    AActor* owner = GetOwner();
    FVector location = owner->GetActorLocation() + owner->GetActorForwardVector() * UKismetMathLibrary::RandomFloatInRange(2500, 5000);
    GetWorld()->SpawnActor<AActor>(GetDefault<UManagerSettings>()->Illusion.LoadSynchronous(), location, FRotator());
}

void UHallucinationComp::SkyChange()
{
    UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().Sky, FPayloadKey(TEXT("Random")));
}

//STATIC TRIGGER-BASED HALLUCINATIONS

void UHallucinationComp::BeginPlay()
{
    Super::BeginPlay();
    UGameplayMessageSubsystem::Get(this).RegisterListener(FChannelTags::Get().Hallucination, this, &UHallucinationComp::ProcessHallucination);
}

void UHallucinationComp::ProcessHallucination(FGameplayTag Channel, const FPayloadKey& Payload)
{
  FName Key = Payload.Key;
  if (!PlayedHallucinationKeys.Contains(Key) && DA->Hallucinations.Contains(Key))
  {
    PlayedHallucinationKeys.Add(Key);
    FHallucination Data = DA->Hallucinations.FindRef(Key);
    Cast<AAvatar>(GetOwner())->AvatarUI->PlayHallucination(Data);
  }
}

bool UHallucinationComp::IsIllusionValid(const FName Key)
{
  if (!PlayedIllusionKeys.Contains(Key))
  {
    PlayedIllusionKeys.Add(Key);
    return true;
  }
  return false;
}
