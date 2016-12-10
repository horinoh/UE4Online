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
�}�b�`�X�e�[�g���ς��� AGameMode::HandleMatchXXX() �n���R�[�������
�� AGameSession::HandleMatchXXX() �n���R�[�������
�� APlayerController::ClientStartOnlineSession(), ClientEndOnlineSession() ���R�[�������
*/
void AOnlineGameMode::DefaultTimer()
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

		//!< #MY_TODO �^�C���A�b�v�̏���
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
