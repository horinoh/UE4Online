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

	//!< SWidget
	virtual bool SupportsKeyboardFocus() const override { return true; }
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override { return FReply::Handled().ReleaseMouseCapture(); }
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override { return FReply::Handled().SetUserFocus(SharedThis(this), EKeyboardFocusCause::SetDirectly); }

	void Construct(const FArguments& InArgs);

	ECheckBoxState IsLanCheckBoxChecked() const;
	void OnIsLanCheckBoxStateChanged(ECheckBoxState CheckBoxState);

	FReply OnOKButtonClicked();
	void OnOKButtonPressed() { OnOKButtonClicked(); }
	FReply OnCancelButtonClicked();
	void OnCancelButtonPressed() { OnCancelButtonClicked(); }

protected:
	TWeakObjectPtr<ULocalPlayer> LocalPlayer;

	bool bIsLanCheckBoxState;
};
