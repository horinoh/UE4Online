// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	void Construct(const FArguments& InArgs);

	struct FServerEntry
	{
		FString Name;
		int32 Dummy;
	};
	TSharedRef<ITableRow> OnServerEntryGenerateRow(TSharedPtr<FServerEntry> Item, const TSharedRef<STableViewBase>& OwnerTable);
	void OnServerEntrySelectionChanged(TSharedPtr<FServerEntry> InItem, ESelectInfo::Type SelectInfo);
	void OnServerEntryMouseButtonDoubleClicked(TSharedPtr<FServerEntry> InItem);

protected:
	TWeakObjectPtr<ULocalPlayer> LocalPlayer;

	TArray<TSharedPtr<FServerEntry> > ServerList;
};
