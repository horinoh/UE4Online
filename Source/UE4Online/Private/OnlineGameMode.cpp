// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineGameMode.h"

#include "OnlineGameSession.h"
#include "OnlineCharacter.h"

AOnlineGameMode::AOnlineGameMode()
{
	DefaultPawnClass = AOnlineCharacter::StaticClass();
}

void AOnlineGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	FTimerHandle TimerHandle_DefaultTimer;
	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AOnlineGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}
void AOnlineGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
TSubclassOf<AGameSession> AOnlineGameMode::GetGameSessionClass() const
{
	return AOnlineGameSession::StaticClass();
}

/**
マッチステートが変わると AGameMode::HandleMatchXXX() 系がコールされる
→ AGameSession::HandleMatchXXX() 系がコールされる
→ APlayerController::ClientStartOnlineSession(), ClientEndOnlineSession() がコールされる
*/
void AOnlineGameMode::DefaultTimer()
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
