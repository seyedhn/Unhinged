#include "TrialStart.h"

void ATrialStart::BindToGizmo()
{
  UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
  Handle = MessageSubsystem.RegisterListener(FChannelTags::Get().Trial, this, &ThisClass::OnTrialAcceptedOrDeclined);
}

void ATrialStart::UnbindFromGizmo()
{
  UGameplayMessageSubsystem::Get(this).UnregisterListener(Handle);
}

void ATrialStart::OnTrialAcceptedOrDeclined(FGameplayTag Channel, const FPayloadBool& Payload)
{
  if (Payload.Activate) StartTrial();
  UnbindFromGizmo();

}
