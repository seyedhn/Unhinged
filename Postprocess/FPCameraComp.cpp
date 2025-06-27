#include "FPCameraComp.h"

void UFPCameraComp::SetFoV(const float FoV)
{
  DefaultFoV = FoV;
  SetFieldOfView(FoV);
}
