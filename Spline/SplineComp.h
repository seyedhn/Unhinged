#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "../Block/BlockSMC.h"
#include "SplineComp.generated.h"

class ABlockActor;

UENUM(BlueprintType)
enum class ESplineShape : uint8
{
	Linear	 UMETA(DisplayName = "Linear"),
	Cubic  UMETA(DisplayName = "Cubic"),
};


UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class THEGAME_API USplineComp : public USplineComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void Initialise(UBlockSMC* StartMeshComp, UBlockSMC* EndMeshComp, UStaticMesh* SplineMesh, UStaticMesh* AttachedMesh, UStaticMesh* GroundMesh, TEnumAsByte<ESplineMeshAxis::Type> SplineMeshAxis,
		ESplineShape SplineShape, EBlockSocketType SocketType, float EndPointLength, float SectionLength, FVector2D SplineScale, EWalkableSlopeBehavior Walkable);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateEndMeshes(bool bEndMesh, bool bAttached);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void GenerateNewSpline();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ActivateSplineCollision();

	UFUNCTION(BlueprintCallable)
	void SetEndMeshesOnLoad();

	UFUNCTION(BlueprintCallable)
	void SetFlowDirection(int32 Direction);

	UFUNCTION(BlueprintCallable)
	void GetInputOutputMeshes(UBlockSMC*& Input, UBlockSMC*& Output) const;


	UPROPERTY(BlueprintReadWrite)
	TArray<USplineMeshComponent*> SplineMeshComponents;

	UPROPERTY(BlueprintReadWrite)
	float MaxBend;

protected:

	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bIsStartMeshAttached;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bIsEndMeshAttached;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	int32 FlowDirection;

	//DEFAULTS//

	UPROPERTY(BlueprintReadOnly)
	ABlockActor* Owner;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UBlockSMC* StartMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UBlockSMC* EndMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMesh* SplineMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMesh* GroundMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMesh* AttachedMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TEnumAsByte<ESplineMeshAxis::Type> SplineMeshAxis;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	ESplineShape SplineShape;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EBlockSocketType SplineSocketType;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float EndPointLength;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float SectionLength;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FVector2D SplineScale;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TEnumAsByte<EWalkableSlopeBehavior> Walkable;

};
