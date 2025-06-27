#include "SpaceChargeSMC.h"

bool USpaceChargeSMC::IsConnectionValid(USpaceChargeSMC* other)
{  
  return (uint8)ChargeType + (uint8)other->ChargeType != 1;
}
