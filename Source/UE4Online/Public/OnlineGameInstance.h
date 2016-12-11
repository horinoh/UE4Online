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
	virtual void StartGameInstance() override;
	virtual bool StartPIEGameInstance(ULocalPlayer* LocalPlayer, bool bInSimulateInEditor, bool bAnyBlueprintErrors, bool bStartInSpectatorMode) override;
	virtual bool JoinSession(ULocalPlayer* LocalPlayer, int32 SessionIndexInSearchResults) override;
	virtual bool JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult) override;

	void OnCreateSessionComplete(FName Name, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName Name, bool bWasSuccessful);

	bool CreateSession(ULocalPlayer* LocalPlayer, const FString& InTravelURL);
	bool CreateSession(ULocalPlayer* LocalPlayer, const FString& GameType, const FString& MapName, bool bIsLanMatch);
	bool FindSessions(ULocalPlayer* PlayerOwner, bool bIsLanMatch);
	bool DestroySession();

	TSharedPtr<class FOnlineMainMenu> GetMainMenu() const { return MainMenu; }

protected:
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	FString TravelURL;

	TSharedPtr<class FOnlineMainMenu> MainMenu;
};
