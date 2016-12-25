// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineArenaGameMode.h"

#include "OnlineCharacter.h"
#include "OnlineGameState.h"

AOnlineArenaGameMode::AOnlineArenaGameMode()
{
	DefaultPawnClass = AOnlineCharacter::StaticClass();
	GameStateClass = AOnlineGameState::StaticClass();
}

void AOnlineArenaGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	FTimerHandle TimerHandle_DefaultTimer;
	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AOnlineArenaGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}

void AOnlineArenaGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	const auto GS = Cast<AOnlineGameState>(GameState);
	if (nullptr != GS)
	{
		GS->RemainingTime = 30;
	}
}

void AOnlineArenaGameMode::DefaultTimer()
{
	const auto World = GetWorld();
	if (nullptr != World && World->IsPlayInEditor())
	{
		//!< PIE �̏ꍇ
		if (MatchState::WaitingToStart == GetMatchState())
		{
			StartMatch();
		}
	}
	else
	{
		const auto MatchSteate = GetMatchState();

		const auto GS = Cast<AOnlineGameState>(GameState);
		if (nullptr != GS)
		{
			--GS->RemainingTime;
			if (GS->RemainingTime <= 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("TIME UP"));
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
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::FromInt(GS->RemainingTime));
				if (MatchState::WaitingToStart == MatchState)
				{
					StartMatch();
				}
			}
		}
	}
}
