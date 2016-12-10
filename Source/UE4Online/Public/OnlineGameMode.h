// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "OnlineGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UE4ONLINE_API AOnlineGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AOnlineGameMode();
	
	//!< AActor
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;

	//!< AGameModeBase
	virtual TSubclassOf<AGameSession> GetGameSessionClass() const override;

	virtual void DefaultTimer();

protected:
};
