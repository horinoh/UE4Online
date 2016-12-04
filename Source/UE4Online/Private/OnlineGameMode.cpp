// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineGameMode.h"

#include "OnlineGameSession.h"

AOnlineGameMode::AOnlineGameMode()
{
}

void AOnlineGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

TSubclassOf<AGameSession> AOnlineGameMode::GetGameSessionClass() const
{
	return AOnlineGameSession::StaticClass();
}
