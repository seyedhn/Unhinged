// Copyright 2024 Seyed Nasrollahi. All Rights Reserved.

#include "BlockLibrary.h"
#include "../Manager/ManagerSubsystem.h"

void UBlockLibrary::ConnectSockets(FSocketData socket1, FSocketData socket2)
{
	UManagerSubsystem& ManagerSubsystem = UManagerSubsystem::Get(socket1.Primitive->GetOwner());
	ManagerSubsystem.BlockManager->ConnectSockets(socket1, socket2);
	ManagerSubsystem.BlockManager->AddConnectionSaveData(socket1, socket2);
}

int32 UBlockLibrary::GetSocketInOutDirection(FSocketData socket)
{
	return socket.GetSocketInOutDirection();
}
