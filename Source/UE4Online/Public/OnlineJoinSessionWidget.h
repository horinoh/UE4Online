// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SThrobber.h"
#include "OnlineSubsystemTypes.h"

/**
 * 
 */
class UE4ONLINE_API SOnlineJoinSessionWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SOnlineJoinSessionWidget)
		: _LocalPlayer()
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<ULocalPlayer>, LocalPlayer)
	SLATE_END_ARGS()

	//!< SWidget
	virtual bool SupportsKeyboardFocus() const override { return true; }
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override { return FReply::Handled().ReleaseMouseCapture(); }
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override { return FReply::Handled().SetUserFocus(SharedThis(this), EKeyboardFocusCause::SetDirectly); }
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	
	void Construct(const FArguments& InArgs);

	struct FServerEntry
	{
		FString ServerName;
		FString CurrentPlayerCount;
		FString MaxPlayerCount;
		FString PingInMs;
		int32 Index;
	};
	TSharedRef<ITableRow> OnServerEntryGenerateRow(TSharedPtr<FServerEntry> Item, const TSharedRef<STableViewBase>& OwnerTable);
	//void OnServerEntrySelectionChanged(TSharedPtr<FServerEntry> InItem, ESelectInfo::Type SelectInfo) {}
	void OnServerEntryMouseButtonDoubleClicked(TSharedPtr<FServerEntry> InItem);
	FReply OnCancelButtonClicked();
	void OnCancelButtonPressed() { OnCancelButtonClicked(); }

	void UpdateSearchStatus();

protected:
	TWeakObjectPtr<ULocalPlayer> LocalPlayer;

	TArray<TSharedPtr<FServerEntry> > ServerList;

	TSharedRef<class SOverlay> Overlay = SNew(SOverlay);
	TSharedRef<class SCircularThrobber> CircularThrobber = SNew(SCircularThrobber);
};
