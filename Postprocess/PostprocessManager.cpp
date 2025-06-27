#include "PostprocessManager.h"
#include "Kismet/GameplayStatics.h"
#include "DynamicPPComp.h"



/*

UDynamicPP* APostprocessManager::CreateDynamicPP(UMaterialInterface* Parent, FName OptionalName, EMIDCreationFlags CreationFlags)
{
	UDynamicPP* NewMID = nullptr;

	if (Parent)
	{

		// In editor MIDs need to be created within a persistent object or else they will not be saved.
		// If this MID is created at runtime or specifically marked as transient then put it in the transient package.
		//UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
		UObject* MIDOuter = !EnumHasAnyFlags(CreationFlags, EMIDCreationFlags::Transient) && GetWorld() && !GetWorld()->IsGameWorld() ? GetWorld() : nullptr;
		NewMID = reinterpret_cast<UDynamicPP*>(UDynamicPP::Create(Parent, MIDOuter, OptionalName));
		if (MIDOuter == nullptr)
		{
			NewMID->SetFlags(RF_Transient);
		}
	}
	NewMID->Initialise(NAME_None, FPostprocessStruct());

	return NewMID;
}*/

/*
UDynamicPP* APostprocessManager::CreateDynamicPP(UMaterialInterface* Parent, FName OptionalName, EMIDCreationFlags CreationFlags)
{
	return NewObject<UDynamicPP>();
}
*/

void APostprocessManager::BeginPlay()
{


	TArray<AActor*> PPVols;
	UGameplayStatics::GetAllActorsOfClass(this, APostProcessVolume::StaticClass(), PPVols);

	for (AActor* actor : PPVols)
	{
		APostProcessVolume* PPVol = Cast<APostProcessVolume>(actor);
		if (PPVol->bEnabled)
		{
			PPVolume = PPVol;
			break;
		}
	}

	Super::BeginPlay();

}
