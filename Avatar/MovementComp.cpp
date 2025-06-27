#include "MovementComp.h"

void UMovementComp::FullyDrainStamina()
{
  Stamina = 0.f;
  OnStaminaUpdated.Broadcast(Stamina);
  GetWorld()->GetTimerManager().UnPauseTimer(StaminaTimer);
}
