#include "SettingsPDA.h"

ESettingType USettingsPDA::GetSettingType(FInstancedStruct Setting) const
{
  if (Setting.GetPtr<FSettingsBinary>()) return ESettingType::Boolean;
  if (Setting.GetPtr<FSettingsScalability>()) return ESettingType::Integer;
  if (Setting.GetPtr<FSettingsMP>()) return ESettingType::Integer;
  if (Setting.GetPtr<FSettingsSlider>()) return ESettingType::Decimal;
  return ESettingType::Decimal;
}
