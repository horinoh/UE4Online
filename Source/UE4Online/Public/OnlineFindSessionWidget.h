// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class UE4ONLINE_API SOnlineFindSessionWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SOnlineFindSessionWidget)
		: _LocalPlayer()
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<ULocalPlayer>, LocalPlayer)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	ECheckBoxState IsLanCheckBoxChecked() const;
	void OnIsLanCheckBoxStateChanged(ECheckBoxState CheckBoxState);

	FReply OnOKButtonClicked();
	FReply OnCancelButtonClicked();

protected:
	TWeakObjectPtr<ULocalPlayer> LocalPlayer;

	bool bIsLanCheckBoxState;
};
