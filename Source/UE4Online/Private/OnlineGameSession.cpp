// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineGameSession.h"

AOnlineGameSession::AOnlineGameSession()
{
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &AOnlineGameSession::OnCreateSessionComplete);
		OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &AOnlineGameSession::OnFindSessionsComplete);
		OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &AOnlineGameSession::OnJoinSessionComplete);
		OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &AOnlineGameSession::OnDestroySessionComplete);
	}
}

void AOnlineGameSession::OnCreateSessionComplete(FName InSessionName, bool bWasSuccessful)
{
	UE_LOG(LogUE4Online, Log, TEXT("OnCreateSessionComplete"));
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("OnCreateSessionComplete : ") + InSessionName.ToString());
	}

	const auto OnlineSub = IOnlineSubsystem::Get();
	if (nullptr != OnlineSub)
	{
		const auto Session = OnlineSub->GetSessionInterface();
		if (Session.IsValid())
		{
			Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteHandle);
		}
	}

	CreateSessionCompleteEvent.Broadcast(InSessionName, bWasSuccessful);
}
void AOnlineGameSession::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(LogUE4Online, Log, TEXT("OnFindSessionsComplete"));
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("OnFindSessionsComplete"));
		if (0 == OnlineSessionSearch->SearchResults.Num())
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Not Found"));
		}
		for (const auto& i : OnlineSessionSearch->SearchResults)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, i.Session.GetSessionIdStr());
		}

		switch (OnlineSessionSearch->SearchState)
		{
		case EOnlineAsyncTaskState::InProgress:
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("InProgress"));
			break;
		case EOnlineAsyncTaskState::Done:
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Done"));
			break;
		case EOnlineAsyncTaskState::Failed:
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Failed"));
			break;
		case EOnlineAsyncTaskState::NotStarted:
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("NotStarted"));
			break;
		}
	}

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
void AOnlineGameSession::OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogUE4Online, Log, TEXT("OnJoinSessionComplete"));
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("OnJoinSessionComplete : ") + InSessionName.ToString());

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
void AOnlineGameSession::OnDestroySessionComplete(FName InSessionName, bool bWasSuccessful)
{
	UE_LOG(LogUE4Online, Log, TEXT("OnDestroySessionComplete"));
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("OnDestroySessionComplete : ") + InSessionName.ToString());
	}

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

	DestroySessionCompleteEvent.Broadcast(InSessionName, bWasSuccessful);
}

bool AOnlineGameSession::CreateSession(TSharedPtr<const FUniqueNetId> UserId, FName InSessionName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	const auto OnlineSub = IOnlineSubsystem::Get();
	if (nullptr != OnlineSub)
	{
		SessionParams.SessionName = InSessionName;
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
				OnlineSessionSettings->Set(SETTING_SESSION_TEMPLATE_NAME, FString("Game"), EOnlineDataAdvertisementType::DontAdvertise);
				OnlineSessionSettings->Set(SEARCH_KEYWORDS, FString("Custom"), EOnlineDataAdvertisementType::ViaOnlineService);

				CreateSessionCompleteHandle = Session->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

				UE_LOG(LogUE4Online, Log, TEXT("CreateSession"));
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("CreateSession : ") + SessionParams.SessionName.ToString());

				return Session->CreateSession(*SessionParams.UserId, SessionParams.SessionName, *OnlineSessionSettings);
			}
		}
	}

	return false;
}

void AOnlineGameSession::FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName InSessionName, bool bIsLAN, bool bIsPresence)
{
	const auto OnlineSub = IOnlineSubsystem::Get();
	if (nullptr != OnlineSub)
	{
		SessionParams.SessionName = InSessionName;
		SessionParams.bIsLAN = bIsLAN;
		SessionParams.bIsPresence = bIsPresence;
		SessionParams.UserId = UserId;

		if (SessionParams.UserId.IsValid())
		{
			const auto Session = OnlineSub->GetSessionInterface();
			if (Session.IsValid())
			{
				OnlineSessionSearch = MakeShareable(new FOnlineSessionSearch());
				OnlineSessionSearch->QuerySettings.Set(SEARCH_DEDICATED_ONLY, true, EOnlineComparisonOp::Equals);
				OnlineSessionSearch->QuerySettings.Set(SEARCH_EMPTY_SERVERS_ONLY, true, EOnlineComparisonOp::Equals); 
				OnlineSessionSearch->QuerySettings.Set(SEARCH_KEYWORDS, FString("Custom"), EOnlineComparisonOp::Equals);
				
				auto OnlineSessionSearchRef = OnlineSessionSearch.ToSharedRef();
				FindSessionsCompleteHandle = Session->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

				UE_LOG(LogUE4Online, Log, TEXT("FindSessions"));
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("FindSessions : ") + SessionParams.SessionName.ToString());

				Session->FindSessions(*SessionParams.UserId, OnlineSessionSearchRef);
			}
		}
	}
	else
	{
		OnFindSessionsComplete(false);
	}
}
bool AOnlineGameSession::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName InSessionName, int32 SessionIndexInSearchResults)
{
	if (0 < SessionIndexInSearchResults && SessionIndexInSearchResults < OnlineSessionSearch->SearchResults.Num())
	{
		JoinSession(UserId, InSessionName, OnlineSessionSearch->SearchResults[SessionIndexInSearchResults]);
	}
	return false;
}
bool AOnlineGameSession::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName InSessionName, const FOnlineSessionSearchResult& SearchResult)
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

				UE_LOG(LogUE4Online, Log, TEXT("JoinSession"));
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("JoinSession : ") + InSessionName.ToString());

				return Session->JoinSession(*UserId, InSessionName, SearchResult);
			}
		}
	}
	return false;
}

bool AOnlineGameSession::DestroySession(FName InSessionName)
{
	const auto OnlineSub = IOnlineSubsystem::Get();
	if (nullptr != OnlineSub)
	{
		const auto Session = OnlineSub->GetSessionInterface();
		if (Session.IsValid())
		{
			DestroySessionCompleteHandle = Session->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			UE_LOG(LogUE4Online, Log, TEXT("DestroySession"));
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("DestroySession : ") + InSessionName.ToString());

			return Session->DestroySession(InSessionName);
		}
	}

	return false;
}

