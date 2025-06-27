#include "SocketConstraint.h"

void USocketConstraint::InitializeComponent()
{
  Super::InitializeComponent();

  SetDisableCollision(true);
  SetAngularTwistLimit(ACM_Locked, 1.f);
  SetAngularSwing1Limit (ACM_Locked, 1.f);
  SetAngularSwing2Limit(ACM_Locked, 1.f);
  SetLinearBreakable(false, 2000000);

}
