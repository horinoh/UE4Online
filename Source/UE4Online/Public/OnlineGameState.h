// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "OnlineGameState.generated.h"

/**
 * 
 */
UCLASS()
class UE4ONLINE_API AOnlineGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Transient, Replicated)
	int32 RemainingTime;
};
