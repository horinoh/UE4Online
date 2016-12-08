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

bool AOnlineGameSession::CreateSession(TSharedPtr<const FUniqueNetId> UserId, FName Name, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	const auto OnlineSub = IOnlineSubsystem::Get();
	if (nullptr != OnlineSub)
	{
		SessionParams.SessionName = Name;
		SessionParams.bIsLAN = bIsLAN;
		SessionParams.bIsPresence = bIsPresence;
		SessionParams.UserId = UserId;

		MaxPlayers = MaxNumPlayers;

		if (SessionParams.UserId.IsValid())
		{
			const auto Session = OnlineSub->GetSessionInterface();
			if (Session.IsValid())
			{
				OnlineSessionSettings = MakeShareable(new FOnlineSessionSettings());
				OnlineSessionSettings->Set(SETTING_GAMEMODE, GameType, EOnlineDataAdvertisementType::ViaOnlineService);
				OnlineSessionSettings->Set(SETTING_MAPNAME, MapName, EOnlineDataAdvertisementType::ViaOnlineService);
				OnlineSessionSettings->Set(SETTING_MATCHING_HOPPER, FString("TeamDeathmatch"), EOnlineDataAdvertisementType::DontAdvertise);
				OnlineSessionSettings->Set(SETTING_MATCHING_TIMEOUT, 120.0f, EOnlineDataAdvertisementType::ViaOnlineService);
				OnlineSessionSettings->Set(SETTING_SESSION_TEMPLATE_NAME, FString("GameSession"), EOnlineDataAdvertisementType::DontAdvertise);
				OnlineSessionSettings->Set(SEARCH_KEYWORDS, FString("Custom"), EOnlineDataAdvertisementType::ViaOnlineService);

				CreateSessionCompleteHandle = Session->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
				return Session->CreateSession(*SessionParams.UserId, SessionParams.SessionName, *OnlineSessionSettings);
			}
		}
	}

	return false;
}
bool AOnlineGameSession::StartSession(FName Name)
{
	const auto OnlineSub = IOnlineSubsystem::Get();
	if (nullptr != OnlineSub)
	{
		const auto Session = OnlineSub->GetSessionInterface();
		if (Session.IsValid())
		{
			StartSessionCompleteHandle = Session->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
			return Session->StartSession(Name);
		}
	}

	return false;
}
void AOnlineGameSession::FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName Name, bool bIsLAN, bool bIsPresence)
{
	const auto OnlineSub = IOnlineSubsystem::Get();
	if (nullptr != OnlineSub)
	{
		SessionParams.SessionName = Name;
		SessionParams.bIsLAN = bIsLAN;
		SessionParams.bIsPresence = bIsPresence;
		SessionParams.UserId = UserId;

		if (SessionParams.UserId.IsValid())
		{
			const auto Session = OnlineSub->GetSessionInterface();
			if (Session.IsValid())
			{
				OnlineSessionSearch = MakeShareable(new FOnlineSessionSearch());
				OnlineSessionSearch->QuerySettings.Set(SEARCH_KEYWORDS, FString("Custom"), EOnlineComparisonOp::Equals);

				auto OnlineSessionSearchRef = OnlineSessionSearch.ToSharedRef();

				FindSessionsCompleteHandle = Session->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
				Session->FindSessions(*SessionParams.UserId, OnlineSessionSearchRef);
			}
		}
	}
	else
	{
		OnFindSessionsComplete(false);
	}
}
bool AOnlineGameSession::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName Name, int32 SessionIndexInSearchResults)
{
	if (0 < SessionIndexInSearchResults && SessionIndexInSearchResults < OnlineSessionSearch->SearchResults.Num())
	{
		JoinSession(UserId, Name, OnlineSessionSearch->SearchResults[SessionIndexInSearchResults]);
	}
	return false;
}
bool AOnlineGameSession::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName Name, const FOnlineSessionSearchResult& SearchResult)
{
	const auto OnlineSub = IOnlineSubsystem::Get();
	if (nullptr != OnlineSub)
	{
		if (UserId.IsValid())
		{
			const auto Session = OnlineSub->GetSessionInterface();
			if (Session.IsValid())
			{
				JoinSessionCompleteHandle = Session->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
				return Session->JoinSession(*UserId, Name, SearchResult);
			}
		}
	}
	return false;
}

bool AOnlineGameSession::EndSession(FName Name)
{
	const auto OnlineSub = IOnlineSubsystem::Get();
	if (nullptr != OnlineSub)
	{
		const auto Session = OnlineSub->GetSessionInterface();
		if (Session.IsValid())
		{
			const auto World = GetWorld();
			if (nullptr != World)
			{
				for (auto It = World->GetPlayerControllerIterator(); It; ++It)
				{
					//!< #MY_TODO
					//if(It->IsLocal)
					//It->ClientEndOnlineGame();
				}
			}
			return Session->EndSession(Name);
		}
	}

	return false;
}
bool AOnlineGameSession::DestroySession(FName Name)
{
	const auto OnlineSub = IOnlineSubsystem::Get();
	if (nullptr != OnlineSub)
	{
		const auto Session = OnlineSub->GetSessionInterface();
		if (Session.IsValid())
		{
			DestroySessionCompleteHandle = Session->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
			return Session->DestroySession(Name);
		}
	}

	return false;
}

