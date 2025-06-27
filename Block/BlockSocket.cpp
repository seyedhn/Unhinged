#include "BlockSocket.h"
#include "BlockSMC.h"

bool FSocketData::IsSocketOfType(EBlockSocketType Type) const
{
	FString SocketType = UEnum::GetValueAsString(Type);
	SocketType.ReplaceInline(TEXT("EBlockSocketType::"), TEXT(""));

	return Socket.ToString().Contains(SocketType);
}

bool FSocketData::IsSocketValid() const
{
	return IsValid(Primitive) && !Socket.IsNone();
}

int32 FSocketData::GetSocketInOutDirection() const
{
	if (Socket.ToString().Contains("_In")) return -1;
	if (Socket.ToString().Contains("_Out")) return 1;
	return 0;
}

FSocketSaveData FSocketData::ToSaveData() const
{	
	FGuid actorID = ISaveGameInterface::Execute_GetActorID(Primitive->GetOwner());
	return FSocketSaveData(actorID, Primitive->GetFName(), Socket);	
}