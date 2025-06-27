#include "EnergyConsumerComp.h"
#include "../Resource/ResourceLibrary.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "../Messaging/ChannelTags.h"
#include "../Messaging/PayloadStructs.h"

bool UEnergyConsumerComp::TryConsumeCharge(int32 Amount)
{
    if (UResourceLibrary::TryDeductFromBackpack(this, Charge, Amount))
    {
        return true;
    }
    FString string = FString("Insufficient <Bold> ").Append(Charge.ToString()).Append("</>");
    UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().Warning, FPayloadKey(FName(string)));
    return false;
}
