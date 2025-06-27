#include "CollisionSoundsPDA.h"


TSoftObjectPtr<USoundBase> UCollisionSoundsPDA::GetCollisionSound(UPhysicalMaterial* PhysMat1, UPhysicalMaterial* PhysMat2) const
{

  if (PhysMat1 && PhysMat2)
  {
    FCollisionPair pair(PhysMat1->SurfaceType, PhysMat2->SurfaceType);   
    if (CollisionSounds.Contains(pair))
    {
      return CollisionSounds.Find(pair)->CollisionSound;
    }
  }

  if (PhysMat1)
  {
    FCollisionSingle single(PhysMat1->SurfaceType);
    if (SingleMatCollisionSounds.Contains(single))
    {
      return SingleMatCollisionSounds.Find(single)->CollisionSound;
    }
  }

  if (PhysMat2)
  {
    FCollisionSingle single(PhysMat2->SurfaceType);
    if (SingleMatCollisionSounds.Contains(single))
    {
      return SingleMatCollisionSounds.Find(single)->CollisionSound;
    }
  }

  return nullptr;
}

TSoftObjectPtr<USoundBase> UCollisionSoundsPDA::GetFrictionSound(UPhysicalMaterial* PhysMat) const
{
  if (PhysMat)
  {
    FCollisionSingle friction(PhysMat->SurfaceType);
    if (FrictionSounds.Contains(friction))
    {
      return FrictionSounds.Find(friction)->CollisionSound;
    }
  }
  return nullptr;
}
