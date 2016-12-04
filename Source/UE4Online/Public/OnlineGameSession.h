// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Online.h"

#include "GameFramework/GameSession.h"
#include "OnlineGameSession.generated.h"

/**
 * 
 */
UCLASS()
class UE4ONLINE_API AOnlineGameSession : public AGameSession
{
	GENERATED_BODY()
	
public:
	AOnlineGameSession();
	
	void OnCreateSessionComplete(FName Name, bool bWasSuccessful);
	void OnStartSessionComplete(FName Name, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName Name, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName Name, bool bWasSuccessful);

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

	TSharedPtr<class FOnlineSessionSettings> OnlineSessionSettings;
	TSharedPtr<class FOnlineSessionSearch> OnlineSessionSearch;
};
