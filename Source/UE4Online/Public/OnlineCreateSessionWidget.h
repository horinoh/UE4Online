// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class UE4ONLINE_API SOnlineCreateSessionWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SOnlineCreateSessionWidget)
		: _LocalPlayer()
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<ULocalPlayer>, LocalPlayer)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FText GetServerNameText() const;
	void OnServerNameTextChanged(const FText& InLabel);

	FText GetPlayerCountText() const;
	void OnPlayerCountTextChanged(const FText& InLabel);
	FReply OnDecreasePlayerCountButtonClicked();
	FReply OnIncreasePlayerCountButtonClicked();
	
	ECheckBoxState IsLanCheckBoxChecked() const;
	void OnIsLanCheckBoxStateChanged(ECheckBoxState CheckBoxState);
	
	FReply OnOKButtonClicked();
	FReply OnCancelButtonClicked();

protected:
	TWeakObjectPtr<ULocalPlayer> LocalPlayer;

	FString ServerName = "--------";
	uint8 PlayerCount = 1;
	bool bIsLanCheckBoxState;
};
