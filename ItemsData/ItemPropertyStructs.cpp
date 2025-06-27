#include "ItemPropertyStructs.h"

EPurchaseType FItemPurchaseData::GetPurchaseType() const
{
  if (Cost < 0) return EPurchaseType::Sellable;
  if (DeliveredAmount < 0) return EPurchaseType::Upgrade;
  return EPurchaseType::Consumable;
}
