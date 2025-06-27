#include "SplineComp.h"
#include "../Block/BlockActor.h"
#include "../Block/BlockSocket.h"

void USplineComp::Initialise(UBlockSMC* _StartMeshComp, UBlockSMC* _EndMeshComp, UStaticMesh* _SplineMesh, UStaticMesh* _AttachedMesh, UStaticMesh* _GroundMesh, TEnumAsByte<ESplineMeshAxis::Type> _SplineMeshAxis, ESplineShape _SplineShape, EBlockSocketType _SocketType, float _EndPointLength, float _SectionLength, FVector2D _SplineScale, EWalkableSlopeBehavior _Walkable)
{
  StartMesh = _StartMeshComp;
  EndMesh = _EndMeshComp;
  SplineMesh = _SplineMesh;
  AttachedMesh = _AttachedMesh;
  GroundMesh = _GroundMesh;
  SplineMeshAxis = _SplineMeshAxis;
  SplineShape = _SplineShape;
  SplineSocketType = _SocketType;
  EndPointLength = _EndPointLength;
  SectionLength = _SectionLength;
  SplineScale = _SplineScale;
  Walkable = _Walkable;
  Owner = Cast<ABlockActor>(GetOwner());
}

void USplineComp::SetEndMeshesOnLoad()
{
  UpdateEndMeshes(false, bIsStartMeshAttached);
  UpdateEndMeshes(true, bIsEndMeshAttached);
}

void USplineComp::SetFlowDirection(int32 Direction)
{
  FlowDirection = Direction;
}

void USplineComp::GetInputOutputMeshes(UBlockSMC*& Input, UBlockSMC*& Output) const
{
  if (FlowDirection == -1)
  {
    Input = EndMesh;
    Output = StartMesh;
  }
  if (FlowDirection == 1)
  {
    Input = StartMesh;
    Output = EndMesh;
  }
}
