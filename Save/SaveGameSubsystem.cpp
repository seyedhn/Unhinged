#include "SaveGameSubsystem.h"
#include "SaveGameInterface.h"
#include "SaveGameObject.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineMeshComponent.h"
#include "InstancedFoliageActor.h"
#include "../Foliage/FoliageISMC.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "../Manager/ManagerSubsystem.h"
#include "../UserSettings/OurUserSettings.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "../Messaging/PayloadStructs.h"
#include "../Block/SocketConstraint.h"


void USaveGameSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{

	Super::OnWorldBeginPlay(InWorld);

	FString MapName = GetWorld()->GetName();
	CurrentSlotName = FString(TEXT("AutoSave_")).Append(MapName);
	
	UManagerSubsystem& ManagerSubsystem = UManagerSubsystem::Get(this);
	ManagerSubsystem.InitialiseManagerActors();

	UOurUserSettings::GetOurUserSettings()->InitialiseUserSettings(GetWorld());

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());

	if (MapName == "TestLevel" || MapName == "OpenWorld_Main")
	{
		LoadSaveGame();
	}
}

USaveGameSubsystem& USaveGameSubsystem::Get(UObject* WorldContextObject)
{	
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	check(World);
	USaveGameSubsystem* SaveGameSubsystem = UWorld::GetSubsystem<USaveGameSubsystem>(World);
	check(SaveGameSubsystem);
	return *SaveGameSubsystem;
}




/////////////////////////////////////////
/////////////   SAVE     ////////////////
/////////////////////////////////////////

bool USaveGameSubsystem::DoesSaveGameExist() const
{
	return UGameplayStatics::DoesSaveGameExist(CurrentSlotName, 0);
}

void USaveGameSubsystem::WriteSaveGame()
{
	CurrentSaveGame->SavedActors.Empty();
	SaveActors();
	SavePlayer();
	SaveFoliage();
	SaveManagers();
	RenameSaveGame();
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, CurrentSlotName, 0);
	UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().Notification, FPayloadNotification(FText::FromString("<Bold>Recall Timestamp</> made")));
	UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().GameSaved, FPayloadEmpty());
}

void USaveGameSubsystem::DeleteSaveGame()
{
	UGameplayStatics::DeleteGameInSlot(CurrentSlotName, 0);
	if (CurrentSaveGame)
	{
		CurrentSaveGame->SavedActors.Empty();
	}
	ActorIDs.Empty();
}

void USaveGameSubsystem::RenameSaveGame()
{
	if (DoesSaveGameExist())
	{
		TObjectPtr<USaveGameObject> LastSaveGame = Cast<USaveGameObject>(UGameplayStatics::LoadGameFromSlot(CurrentSlotName, 0));
		FString LastSaveSlotName = FString("BackupSave_").Append(FDateTime::Now().ToString());
		UGameplayStatics::SaveGameToSlot(LastSaveGame, LastSaveSlotName, 0);
	}
}

void USaveGameSubsystem::SavePlayer()
{
	APawn* Player = UManagerSubsystem::Get(this).Avatar;  //UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	CurrentSaveGame->SavedPlayer.PlayerTransform = Player->GetActorTransform();
	//CurrentSaveGame->SavedPlayer.ControllerRot = PC->GetControlRotation();
	SaveActorComponents(Player, CurrentSaveGame->SavedPlayer.Components);

}

void USaveGameSubsystem::SaveFoliage()
{
	TArray<AActor*> FoliageActors;		
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInstancedFoliageActor::StaticClass(), FoliageActors);
	for (auto& FoliageActor : FoliageActors)
	{
		TMap<FName, FFoliageCompSaveData> FoliageCompSaveData;
		TArray<UActorComponent*> Comps = FoliageActor->GetComponentsByInterface(USaveGameInterface::StaticClass());
		for (UActorComponent* Comp : Comps)
		{
			/*Foliage serialize works in a weird way when instances are removed, so I couldn't use that method.
			 Instead I had to explicitly save and load foliage variables instead of using SaveGame.*/

			UFoliageISMC* FoliageComp = Cast<UFoliageISMC>(Comp);
			FName FoliageMeshName = FoliageComp->GetStaticMesh().GetFName();
			//SaveByteData(FoliageComp, FoliageSaveData.FoliageByteData);
			FoliageCompSaveData.Add(FoliageMeshName, FoliageComp->GetRemovedInstances());
			ISaveGameInterface::Execute_OnActorSaved(FoliageComp);
		}
		CurrentSaveGame->SavedFoliage.Add(FFoliageSaveData(FoliageActor->GetFName(), FoliageCompSaveData));
	}
}

void USaveGameSubsystem::SaveManagers()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AManagerBase::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		Cast<AManagerBase>(Actor)->SaveManager();

		FManagerSaveData ManagerSaveData;
		SaveByteData(Actor, ManagerSaveData.ManagerByteData);

		CurrentSaveGame->SavedManagers.Add(Actor->GetClass(), ManagerSaveData);
	}

}


void USaveGameSubsystem::SaveActors()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveGameInterface::StaticClass(), Actors);
	for(AActor* Actor : Actors)
	{ 
		ISaveGameInterface::Execute_OnActorSaved(Actor);
		FActorSaveData ActorData;
		SaveActorData(Actor, ActorData);
		SaveActorComponents(Actor, ActorData.Components);
		SaveByteData(Actor, ActorData.ByteData);
		CurrentSaveGame->SavedActors.Add(ActorData); //This causes crash on the BitArray.h FindFirstSetBit() function.


	}
}

void USaveGameSubsystem::SaveActorData(AActor* Actor, FActorSaveData& ActorData)
{
	ActorData.ActorID = ISaveGameInterface::Execute_GetActorID(Actor);
	ActorData.Transform = Actor->GetActorTransform();
	ActorData.ActorClass = Actor->GetClass();
}

void USaveGameSubsystem::SaveActorComponents(AActor* Actor, TSet<FComponentSaveData>& CompsData)
{
	TArray<UActorComponent*> comps;
	if (Actor->Implements<USaveGameInterface>() && ISaveGameInterface::Execute_SaveAllComps(Actor))
		Actor->GetComponents<UActorComponent>(comps);
	else
		comps = Actor->GetComponentsByInterface(USaveGameInterface::StaticClass());


	for (UActorComponent* comp : comps)
	{
		if (comp->GetClass() == USplineMeshComponent::StaticClass()) continue;
		UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(comp);
		FTransform CompTransform = PrimComp ? PrimComp->GetComponentTransform() : FTransform();
		bool IsSimulating = PrimComp ? PrimComp->IsSimulatingPhysics() : false;
		FComponentSaveData CompData(comp->GetFName(), comp->GetClass(), CompTransform, IsSimulating);

		SaveByteData(comp, CompData.ComponentByteData);
		CompsData.Add(CompData);
		if(comp->Implements<USaveGameInterface>()) ISaveGameInterface::Execute_OnActorSaved(comp);
	}
}

void USaveGameSubsystem::SaveByteData(UObject* Object, TArray<uint8>& Data)
{
	FMemoryWriter MemWriter(Data);
	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;
	Ar.ArNoDelta = true;
	Object->Serialize(Ar);
}





/////////////////////////////////////////
/////////////   LOAD     ////////////////
/////////////////////////////////////////

void USaveGameSubsystem::LoadSaveGame(bool RewindLoad)
{

	if(DoesSaveGameExist())
	{
	
		CurrentSaveGame = Cast<USaveGameObject>(UGameplayStatics::LoadGameFromSlot(CurrentSlotName, 0));
		if (!CurrentSaveGame)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}

		LoadActors(RewindLoad);
		LoadFoliage(RewindLoad);
		LoadPlayer(RewindLoad);
		LoadManagers(RewindLoad);

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(FChannelTags::Get().GameLoaded, FPayloadBool(RewindLoad));
	}
	else
	{
		CurrentSaveGame = CastChecked<USaveGameObject>(UGameplayStatics::CreateSaveGameObject(USaveGameObject::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
}


void USaveGameSubsystem::LoadPlayer(bool RewindLoad)
{
	APawn* Player = UManagerSubsystem::Get(this).Avatar;  //UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!RewindLoad)
	{
		Player->SetActorTransform(CurrentSaveGame->SavedPlayer.PlayerTransform);
	}
	//PC->SetControlRotation(CurrentSaveGame->SavedPlayer.ControllerRot);
	UE_LOG(LogTemp, Warning, TEXT("Loading Avatar Components"));
	LoadActorComponents(Player, CurrentSaveGame->SavedPlayer.Components, RewindLoad);
	UE_LOG(LogTemp, Warning, TEXT("Avatar Components Loaded"));
}

void USaveGameSubsystem::LoadFoliage(bool RewindLoad)
{
	TArray<AActor*> FoliageActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInstancedFoliageActor::StaticClass(), FoliageActors);
	for (auto& FoliageActor : FoliageActors)
	{
		if (FFoliageSaveData* FoliageData = CurrentSaveGame->SavedFoliage.Find(FoliageActor->GetFName()))
		{
			TArray<UActorComponent*> Comps = FoliageActor->GetComponentsByInterface(USaveGameInterface::StaticClass());
			for (UActorComponent* Comp : Comps)
			{
				UFoliageISMC* FoliageComp = Cast<UFoliageISMC>(Comp);
				FName FoliageCompName = FoliageComp->GetStaticMesh()->GetFName();
				FFoliageCompSaveData* FoliageCompSaveData = FoliageData->FoliageComps.Find(FoliageCompName);
				//LoadByteData(FoliageComp, FoliageSaveData->FoliageByteData);
				FoliageComp->SetRemovedInstance(FoliageCompSaveData->RemovedInstances);
				ISaveGameInterface::Execute_OnActorReloaded(Comp, RewindLoad);
			}
		}
	}
}

void USaveGameSubsystem::LoadManagers(bool RewindLoad) const
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AManagerBase::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		FManagerSaveData ManagerSaveData = *CurrentSaveGame->SavedManagers.Find(Actor->GetClass());
		AManagerBase* Manager = Cast<AManagerBase>(Actor);
		UE_LOG(LogTemp, Warning, TEXT("Loading Manager: %s"), *Manager->GetFName().ToString());
		Manager->PreLoadManager(RewindLoad);
		LoadByteData(Actor, ManagerSaveData.ManagerByteData);
		Manager->LoadManager(RewindLoad);	
	}
}

void USaveGameSubsystem::LoadActors(bool RewindLoad)
{
	TSet<FActorSaveData> ActorsToSpawn = CurrentSaveGame->SavedActors;
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveGameInterface::StaticClass(), Actors);

	for (AActor* Actor : Actors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Loading Actor: %s"), *Actor->GetFName().ToString());
		FGuid ActorID = ISaveGameInterface::Execute_GetActorID(Actor);
		
		if(FActorSaveData* ActorData = CurrentSaveGame->SavedActors.Find(ActorID))
		{		
			LoadActor(Actor, *ActorData, RewindLoad);
			ActorsToSpawn.Remove(ActorID);
		}
		else
		{
			DestroyActor(Actor);
		}
	}	
	SpawnActors(ActorsToSpawn, RewindLoad);	
}

void USaveGameSubsystem::LoadActor(AActor* Actor, const FActorSaveData& ActorData, bool RewindLoad)
{
	if (Actor->GetRootComponent() && Actor->GetRootComponent()->Mobility == EComponentMobility::Movable)
	{
		Actor->SetActorTransform(ActorData.Transform);
	}
	LoadActorComponents(Actor, ActorData.Components, RewindLoad);
	LoadByteData(Actor, ActorData.ByteData);
	ActorIDs.Add(ActorData.ActorID, Actor);
	ISaveGameInterface::Execute_OnActorReloaded(Actor, RewindLoad);
}

void USaveGameSubsystem::LoadActorComponents(AActor* Actor, const TSet<FComponentSaveData>& CompsData, bool RewindLoad)
{

	TSet<FComponentSaveData> ComponentsToSpawn = CompsData;
	TArray<UActorComponent*> comps;
	if(Actor->Implements<USaveGameInterface>() && ISaveGameInterface::Execute_SaveAllComps(Actor))
		Actor->GetComponents<UActorComponent>(comps);
	else
		comps = Actor->GetComponentsByInterface(USaveGameInterface::StaticClass());
	
	
	for (UActorComponent* comp : comps)
	{
		if (const FComponentSaveData* CompData = CompsData.Find(comp->GetFName()))
		{
			ComponentsToSpawn.Remove(CompData->ComponentName);

			if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(comp))
			{
				if (PrimComp->Mobility == EComponentMobility::Movable)
				{
					PrimComp->SetWorldTransform(CompData->ComponentTransform);
					PrimComp->SetSimulatePhysics(CompData->IsSimulating);
				}
			}
			LoadByteData(comp, CompData->ComponentByteData);
			if(comp->Implements<USaveGameInterface>()) ISaveGameInterface::Execute_OnActorReloaded(comp, RewindLoad);
		}
		else
		{
			comp->DestroyComponent();
		}	
	}
	SpawnComponents(Actor, ComponentsToSpawn, RewindLoad);
}



void USaveGameSubsystem::LoadByteData(UObject* Object, const TArray<uint8>& Data) const
{
	FMemoryReader MemReader(Data);
	FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
	Ar.ArIsSaveGame = true;
	Ar.ArNoDelta = true;
	Object->Serialize(Ar);
}

/////////////////////////////////////////
/////////////   ACTOR     ///////////////
/////////////////////////////////////////

void USaveGameSubsystem::SpawnActors(TSet<FActorSaveData>& ActorsToSpawn, bool RewindLoad)
{
	for (FActorSaveData& ActorData : ActorsToSpawn)
	{
		AActor* Actor = GetWorld()->SpawnActor(ActorData.ActorClass, &ActorData.Transform, FActorSpawnParameters());
		ISaveGameInterface::Execute_SetActorID(Actor, ActorData.ActorID); //This is to ensure the same original ActorID is assigned to spawned actors.
		LoadActorComponents(Actor, ActorData.Components);
		LoadByteData(Actor, ActorData.ByteData);
		ActorIDs.Add(ActorData.ActorID, Actor);
		
		if (ISaveGameInterface* SaveInterface = Cast<ISaveGameInterface>(Actor))
		{
			SaveInterface->Execute_OnActorSpawnLoaded(Actor, RewindLoad);
			SaveInterface->OnActorSpawnLoaded_Internal(RewindLoad);
		}
		
	}
}

void USaveGameSubsystem::SpawnComponents(AActor* Owner, TSet<FComponentSaveData>& CompsToSpawn, bool RewindLoad)
{
	for (FComponentSaveData& CompData : CompsToSpawn)
	{
		//This is a poor approach. Ideally we want a more systemic way of disallowing types, perhaps an interface to be implemented by those classes
		if (CompData.ComponentClass == USplineMeshComponent::StaticClass() || CompData.ComponentClass->IsChildOf<UPhysicsConstraintComponent>() || CompData.ComponentClass->IsChildOf<UAudioComponent>()) continue;

		UActorComponent* Comp = Owner->AddComponentByClass(CompData.ComponentClass, true, CompData.ComponentTransform, false);
		if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Comp))
		{
			PrimComp->SetMobility(EComponentMobility::Movable);
			PrimComp->SetSimulatePhysics(CompData.IsSimulating);
		}
		LoadByteData(Comp, CompData.ComponentByteData);
		Comp->Rename(*CompData.ComponentName.ToString());
		if (Comp->Implements<USaveGameInterface>())
		{
			ISaveGameInterface::Execute_OnActorSpawnLoaded(Comp, RewindLoad);
			//ISaveGameInterface::Execute_OnActorSpawnLoaded_Internal(Comp, RewindLoad);
		}
	}
}

AActor* USaveGameSubsystem::GetActorFromID(const FGuid& ActorID)
{
	if (AActor** Actor = ActorIDs.Find(ActorID))
	{
		return *Actor;
	}
	return nullptr;
}

void USaveGameSubsystem::DestroyActor(AActor* ActorToDestroy)
{
	ActorToDestroy->Destroy();
}