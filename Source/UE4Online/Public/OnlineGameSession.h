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
	//virtual void HandleMatchHasStarted() override;
	//virtual void HandleMatchHasEnded() override;
	//virtual void OnStartSessionComplete(FName InSessionName, bool bWasSuccessful) override;
	//virtual void OnEndSessionComplete(FName InSessionName, bool bWasSuccessful) override;

	//void OnStartSessionComplete_(FName InSessionName, bool bWasSuccessful) {}
	//void OnEndSessionComplete_(FName InSessionName, bool bWasSuccessful) {}
	void OnCreateSessionComplete(FName InSessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName InSessionName, bool bWasSuccessful);

	//bool StartSession(FName InSessionName);
	//bool EndSession(FName InSessionName);
	bool CreateSession(TSharedPtr<const FUniqueNetId> UserId, FName InSessionName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName InSessionName, bool bIsLAN, bool bIsPresence);
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName InSessionName, int32 SessionIndexInSearchResults);
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName InSessionName, const FOnlineSessionSearchResult& SearchResult);
	bool DestroySession(FName InSessionName);

	//DECLARE_EVENT_TwoParams(AShooterGameSession, FOnStartSessionComplete, FName, bool);
	//FOnStartSessionComplete StartSessionCompleteEvent;
	//DECLARE_EVENT_TwoParams(AShooterGameSession, FOnEndSessionComplete, FName, bool);
	//FOnEndSessionComplete EndSessionCompleteEvent;
	DECLARE_EVENT_TwoParams(AShooterGameSession, FOnCreateSessionComplete, FName, bool);
	FOnCreateSessionComplete CreateSessionCompleteEvent;
	DECLARE_EVENT_OneParam(AShooterGameSession, FOnFindSessionsComplete, bool);
	FOnFindSessionsComplete FindSessionsCompleteEvent;
	DECLARE_EVENT_OneParam(AShooterGameSession, FOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type);
	FOnJoinSessionComplete JoinSessionCompleteEvent;
	DECLARE_EVENT_TwoParams(AShooterGameSession, FOnDestroySessionComplete, FName, bool);
	FOnDestroySessionComplete DestroySessionCompleteEvent;

	//FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	//FOnEndSessionCompleteDelegate OnEndSessionCompleteDelegate;
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	//FDelegateHandle StartSessionCompleteHandle;
	//FDelegateHandle EndSessionCompleteHandle;
	FDelegateHandle CreateSessionCompleteHandle;
	FDelegateHandle FindSessionsCompleteHandle;
	FDelegateHandle JoinSessionCompleteHandle;
	FDelegateHandle DestroySessionCompleteHandle;

	FSessionParams SessionParams;
	TSharedPtr<class FOnlineSessionSettings> OnlineSessionSettings;
	TSharedPtr<class FOnlineSessionSearch> OnlineSessionSearch;
};
