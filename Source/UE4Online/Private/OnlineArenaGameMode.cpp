// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineArenaGameMode.h"

#include "OnlineCharacter.h"

AOnlineArenaGameMode::AOnlineArenaGameMode()
{
	DefaultPawnClass = AOnlineCharacter::StaticClass();
}

void AOnlineArenaGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	FTimerHandle TimerHandle_DefaultTimer;
	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AOnlineArenaGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}

void AOnlineArenaGameMode::DefaultTimer()
{
	const auto World = GetWorld();
	if (nullptr != World && World->IsPlayInEditor())
	{
		//!< PIE の場合
		if (MatchState::WaitingToStart == GetMatchState())
		{
			StartMatch();
		}
	}
	else
	{
		const auto MatchSteate = GetMatchState();

		//!< #MY_TODO タイムアップの処理
		if (false)
		{
			if (MatchState::WaitingPostMatch == MatchState)
			{
				RestartGame();
			}
			else if (MatchState::InProgress == MatchState)
			{
				EndMatch();
			}
		}
		else
		{
			if (MatchState::WaitingToStart == MatchState)
			{
				StartMatch();
			}
		}
	}
}
