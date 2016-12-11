// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Online.h"

#include "GameFramework/GameSession.h"
#include "OnlineGameSession.generated.h"

struct FSessionParams
{
	FName SessionName = NAME_None;
	bool bIsLAN;
	bool bIsPresence;
	TSharedPtr<const FUniqueNetId> UserId;
};

/**
 * 
 */
UCLASS()
class UE4ONLINE_API AOnlineGameSession : public AGameSession
{
	GENERATED_BODY()
	
public:
	AOnlineGameSession();

	void OnCreateSessionComplete(FName InSessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName InSessionName, bool bWasSuccessful);

	bool CreateSession(TSharedPtr<const FUniqueNetId> UserId, FName InSessionName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName InSessionName, bool bIsLAN, bool bIsPresence);
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName InSessionName, int32 SessionIndexInSearchResults);
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName InSessionName, const FOnlineSessionSearchResult& SearchResult);
	bool DestroySession(FName InSessionName);

	TSharedPtr<class FOnlineSessionSettings> GetOnlineSessionSettings() const { return OnlineSessionSettings; }
	TSharedPtr<class FOnlineSessionSearch> GetOnlineSessionSearch() const { return OnlineSessionSearch; }

//protected:
	DECLARE_EVENT_TwoParams(AShooterGameSession, FOnCreateSessionComplete, FName, bool);
	FOnCreateSessionComplete CreateSessionCompleteEvent;
	DECLARE_EVENT_OneParam(AShooterGameSession, FOnFindSessionsComplete, bool);
	FOnFindSessionsComplete FindSessionsCompleteEvent;
	DECLARE_EVENT_OneParam(AShooterGameSession, FOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type);
	FOnJoinSessionComplete JoinSessionCompleteEvent;
	DECLARE_EVENT_TwoParams(AShooterGameSession, FOnDestroySessionComplete, FName, bool);
	FOnDestroySessionComplete DestroySessionCompleteEvent;

protected:
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	FDelegateHandle CreateSessionCompleteHandle;
	FDelegateHandle FindSessionsCompleteHandle;
	FDelegateHandle JoinSessionCompleteHandle;
	FDelegateHandle DestroySessionCompleteHandle;

	FSessionParams SessionParams;
//public:
	TSharedPtr<class FOnlineSessionSettings> OnlineSessionSettings;
	TSharedPtr<class FOnlineSessionSearch> OnlineSessionSearch;
};
