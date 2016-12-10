// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class UE4ONLINE_API SOnlineMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SOnlineMenuWidget)
		: _LocalPlayer()
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<ULocalPlayer>, LocalPlayer)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FReply OnCreateSessionButtonClicked();
	FReply OnFindSessionButtonClicked();
	FReply OnQuitButtonClicked();

protected:
	TWeakObjectPtr<ULocalPlayer> LocalPlayer;
};
