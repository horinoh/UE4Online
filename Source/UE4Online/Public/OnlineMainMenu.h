// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class UE4ONLINE_API FOnlineMainMenu : public TSharedFromThis<FOnlineMainMenu>, public FTickableGameObject
{
public:

	//!< FTickableObjectBase
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return true; }
	virtual TStatId GetStatId() const override;

	void Construct(TWeakObjectPtr<class UOnlineGameInstance> GI, TWeakObjectPtr<ULocalPlayer> LP);

protected:
	TWeakObjectPtr<class UOnlineGameInstance> GameInstance;
	TWeakObjectPtr<ULocalPlayer> LocalPlayer;
public:
	TSharedPtr<class SOnlineMenuWidget> MenuWidget;
	TSharedPtr<class SWeakWidget> MenuWidgetContainer;

	TSharedPtr<class SOnlineCreateSessionWidget> CreateSessionWidget;
	TSharedPtr<class SWeakWidget> CreateSessionWidgetContainer;

	TSharedPtr<class SOnlineFindSessionWidget> FindSessionWidget;
	TSharedPtr<class SWeakWidget> FindSessionWidgetContainer;

	TSharedPtr<class SOnlineJoinSessionWidget> JoinSessionWidget;
	TSharedPtr<class SWeakWidget> JoinSessionWidgetContainer;
};
