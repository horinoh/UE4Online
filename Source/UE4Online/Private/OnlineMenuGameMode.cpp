// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineMenuGameMode.h"

#include "OnlineGameSession.h"

TSubclassOf<AGameSession> AOnlineMenuGameMode::GetGameSessionClass() const
{
	return AOnlineGameSession::StaticClass();
}
