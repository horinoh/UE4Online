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

	//!< AGameSession
	virtual void HandleMatchHasStarted() override { StartSession(GameSessionName); }
	virtual void HandleMatchHasEnded() override { EndSession(GameSessionName); }
	
	void OnCreateSessionComplete(FName Name, bool bWasSuccessful);
	void OnStartSessionComplete(FName Name, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName Name, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName Name, bool bWasSuccessful);

	bool CreateSession(TSharedPtr<const FUniqueNetId> UserId, FName Name, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
	bool StartSession(FName Name);
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName Name, bool bIsLAN, bool bIsPresence);
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName Name, const FOnlineSessionSearchResult& SearchResult);
	bool EndSession(FName Name);
	bool DestroySession(FName Name);

	DECLARE_EVENT_TwoParams(AShooterGameSession, FOnCreateSessionComplete, FName, bool);
	FOnCreateSessionComplete CreateSessionCompleteEvent;
	DECLARE_EVENT_TwoParams(AShooterGameSession, FOnStartSessionComplete, FName, bool);
	FOnStartSessionComplete StartSessionCompleteEvent;
	DECLARE_EVENT_OneParam(AShooterGameSession, FOnFindSessionsComplete, bool);
	FOnFindSessionsComplete FindSessionsCompleteEvent;
	DECLARE_EVENT_OneParam(AShooterGameSession, FOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type);
	FOnJoinSessionComplete JoinSessionCompleteEvent;
	DECLARE_EVENT_TwoParams(AShooterGameSession, FOnDestroySessionComplete, FName, bool);
	FOnDestroySessionComplete DestroySessionCompleteEvent;

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	FDelegateHandle CreateSessionCompleteHandle;
	FDelegateHandle StartSessionCompleteHandle;
	FDelegateHandle FindSessionsCompleteHandle;
	FDelegateHandle JoinSessionCompleteHandle;
	FDelegateHandle DestroySessionCompleteHandle;

	FSessionParams SessionParams;
	TSharedPtr<class FOnlineSessionSettings> OnlineSessionSettings;
	TSharedPtr<class FOnlineSessionSearch> OnlineSessionSearch;
};
