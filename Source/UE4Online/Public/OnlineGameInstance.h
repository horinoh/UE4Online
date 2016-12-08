// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Online.h"

#include "Engine/GameInstance.h"
#include "OnlineGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE4ONLINE_API UOnlineGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	//!< UGameInstance
	virtual void Init() override;
	virtual bool JoinSession(ULocalPlayer* LocalPlayer, int32 SessionIndexInSearchResults) override;
	virtual bool JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult) override;

	void OnCreateSessionComplete(FName Name, bool bWasSuccessful);
	void OnStartSessionComplete(FName Name, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName Name, bool bWasSuccessful);

	//void OnEndSessionComplete(FName Name, bool bWasSuccessful);

	bool CreateSession(ULocalPlayer* LocalPlayer, const FString& InTravelURL);
	bool CreateSession(ULocalPlayer* LocalPlayer, const FString& GameType, const FString& MapName, bool bIsLAN);
	bool StartSession();
	bool FindSessions(ULocalPlayer* PlayerOwner, bool bIsLAN);
	bool DestroySession();

	virtual bool StartPIEGameInstance(ULocalPlayer* LocalPlayer, bool bInSimulateInEditor, bool bAnyBlueprintErrors, bool bStartInSpectatorMode) override;

protected:
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	TSharedPtr<class FOnlineMainMenu> MainMenu;
};
