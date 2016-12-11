// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "OnlineArenaGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UE4ONLINE_API AOnlineArenaGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AOnlineArenaGameMode();

	//!< AActor
	virtual void PreInitializeComponents() override;

	virtual void DefaultTimer();
};
