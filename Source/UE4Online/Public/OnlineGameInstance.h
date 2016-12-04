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

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	//void OnEndSessionComplete(FName SessionName, bool bWasSuccessful);

protected:
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
};
