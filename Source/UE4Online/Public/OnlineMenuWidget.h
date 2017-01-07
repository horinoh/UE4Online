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

	//!< SWidget
	virtual bool SupportsKeyboardFocus() const override { return true; }
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override { return FReply::Handled().ReleaseMouseCapture(); }
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override { return FReply::Handled().SetUserFocus(SharedThis(this), EKeyboardFocusCause::SetDirectly); }

	void Construct(const FArguments& InArgs);

	FReply OnCreateSessionButtonClicked();
	void OnCreateSessionButtonPressed() { OnCreateSessionButtonClicked(); }
	FReply OnFindSessionButtonClicked();
	void OnFindSessionButtonPressed() { OnFindSessionButtonClicked(); }
	FReply OnQuitButtonClicked();
	void OnQuitButtonPressed() { OnQuitButtonClicked(); }

protected:
	TWeakObjectPtr<ULocalPlayer> LocalPlayer;
};
