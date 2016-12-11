// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "OnlineMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UE4ONLINE_API AOnlineMenuGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	//!< AGameModeBase
	virtual TSubclassOf<AGameSession> GetGameSessionClass() const override;
};
