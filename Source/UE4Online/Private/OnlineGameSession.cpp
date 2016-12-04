// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineGameSession.h"

AOnlineGameSession::AOnlineGameSession()
{
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &AOnlineGameSession::OnCreateSessionComplete);
		OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &AOnlineGameSession::OnStartSessionComplete);
		OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &AOnlineGameSession::OnDestroySessionComplete);
		OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &AOnlineGameSession::OnFindSessionsComplete);
		OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &AOnlineGameSession::OnJoinSessionComplete);
	}
}

void AOnlineGameSession::OnCreateSessionComplete(FName Name, bool bWasSuccessful)
{
	UE_LOG(LogUE4Online, Log, TEXT("OnCreateSessionComplete"));

	const auto OnlineSub = IOnlineSubsystem::Get();
	if (nullptr != OnlineSub)
	{
		const auto Session = OnlineSub->GetSessionInterface();
		if (Session.IsValid())
		{
			Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteHandle);
		}
	}

	CreateSessionCompleteEvent.Broadcast(Name, bWasSuccessful);
}
void AOnlineGameSession::OnStartSessionComplete(FName Name, bool bWasSuccessful)
{
	UE_LOG(LogUE4Online, Log, TEXT("OnStartSessionComplete"));

	const auto OnlineSub = IOnlineSubsystem::Get();
	if (nullptr != OnlineSub)
	{
		const auto Session = OnlineSub->GetSessionInterface();
		if (Session.IsValid())
		{
			Session->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteHandle);
		}
	}

	if (bWasSuccessful)
	{
		const auto World = GetWorld();
		if (nullptr != World)
		{
			for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
			{
				const auto PC = Cast<APlayerController>(*It);
				if (nullptr != PC && !PC->IsLocalPlayerController())
				{
					PC->ClientStartOnlineSession();
				}
			}
		}
	}

	StartSessionCompleteEvent.Broadcast(Name, bWasSuccessful);
}
void AOnlineGameSession::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(LogUE4Online, Log, TEXT("OnFindSessionsComplete"));

	const auto OnlineSub = IOnlineSubsystem::Get();
	if (nullptr != OnlineSub)
	{
		const auto Session = OnlineSub->GetSessionInterface();
		if (Session.IsValid())
		{
			Session->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteHandle);
		}
	}

	for (const auto& i : OnlineSessionSearch->SearchResults)
	{
		DumpSession(&i.Session);
	}

	FindSessionsCompleteEvent.Broadcast(bWasSuccessful);
}
void AOnlineGameSession::OnJoinSessionComplete(FName Name, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogUE4Online, Log, TEXT("OnJoinSessionComplete"));

	const auto OnlineSub = IOnlineSubsystem::Get();
	if (nullptr != OnlineSub)
	{
		const auto Session = OnlineSub->GetSessionInterface();
		if (Session.IsValid())
		{
			Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteHandle);
		}
	}

	JoinSessionCompleteEvent.Broadcast(Result);
}
void AOnlineGameSession::OnDestroySessionComplete(FName Name, bool bWasSuccessful)
{
	UE_LOG(LogUE4Online, Log, TEXT("OnDestroySessionComplete"));

	const auto OnlineSub = IOnlineSubsystem::Get();
	if (nullptr != OnlineSub)
	{
		const auto Session = OnlineSub->GetSessionInterface();
		if (Session.IsValid())
		{
			Session->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteHandle);

			OnlineSessionSettings = nullptr;
		}
	}

	DestroySessionCompleteEvent.Broadcast(Name, bWasSuccessful);
}