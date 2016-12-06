// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineJoinSessionWidget.h"

#define LOCTEXT_NAMESPACE "JoinSessionWidget"

void SOnlineJoinSessionWidget::Construct(const FArguments& InArgs)
{
	LocalPlayer = InArgs._LocalPlayer;

	ServerList.Add(MakeShareable(new FServerEntry({ "AAA", 000 })));
	ServerList.Add(MakeShareable(new FServerEntry({ "BBB", 111 })));
	ServerList.Add(MakeShareable(new FServerEntry({ "CCC", 222 })));
	ServerList.Add(MakeShareable(new FServerEntry({ "DDD", 333 })));
	ServerList.Add(MakeShareable(new FServerEntry({ "EEE", 444 })));
	ServerList.Add(MakeShareable(new FServerEntry({ "FFF", 555 })));

	//!< タイトル
	const auto TitleTextBlock = SNew(STextBlock)
		.Text(LOCTEXT("JOIN_SESSION_TITLE_Key", "Join Session Menu"))
		.ToolTipText(LOCTEXT("JOIN_SESSION_TITLE_TIP_Key", "Join Session Menu"));

	//!< サーバリスト
	const auto ServerListView = SNew(SListView<TSharedPtr<FServerEntry> >)
		.ItemHeight(20)
		.ListItemsSource(&ServerList)
		.SelectionMode(ESelectionMode::Single)
		.OnGenerateRow(this, &SOnlineJoinSessionWidget::OnServerEntryGenerateRow)
		.OnSelectionChanged(this, &SOnlineJoinSessionWidget::OnServerEntrySelectionChanged)
		.OnMouseButtonDoubleClick(this, &SOnlineJoinSessionWidget::OnServerEntryMouseButtonDoubleClicked)
		.HeaderRow(
			SNew(SHeaderRow)
			+ SHeaderRow::Column("Name").FixedWidth(100).DefaultLabel(LOCTEXT("NameColumn", "Server Name"))
			+ SHeaderRow::Column("Dummy").DefaultLabel(LOCTEXT("DummyColumn", "Dummy"))
		);

	const auto VBox = SNew(SVerticalBox) + SVerticalBox::Slot();
	VBox->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			TitleTextBlock
		];
	VBox->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			ServerListView
		];

	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			VBox
		];
}

TSharedRef<ITableRow> SOnlineJoinSessionWidget::OnServerEntryGenerateRow(TSharedPtr<FServerEntry> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	class SEntryWidget : public SMultiColumnTableRow<TSharedPtr<FServerEntry> >
	{
	public:
		SLATE_BEGIN_ARGS(SEntryWidget) {}
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable, TSharedPtr<FServerEntry> InItem)
		{
			Item = InItem;
			SMultiColumnTableRow<TSharedPtr<FServerEntry> >::Construct(FSuperRowType::FArguments(), InOwnerTable);
		}

		TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName)
		{
			auto ItemText = FText::GetEmpty();
			if (ColumnName == "Name")
			{
				ItemText = FText::FromString(Item->Name);
			}
			else if (ColumnName == "Dummy")
			{
				ItemText = FText::FromString(FString::FromInt(Item->Dummy));
			}
			return SNew(STextBlock).Text(ItemText);
		}

	protected:
		TSharedPtr<FServerEntry> Item;
	};

	return SNew(SEntryWidget, OwnerTable, Item);
}
void SOnlineJoinSessionWidget::OnServerEntryMouseButtonDoubleClicked(TSharedPtr<FServerEntry> InItem)
{
	//!< #MY_TODO
}
void SOnlineJoinSessionWidget::OnServerEntrySelectionChanged(TSharedPtr<FServerEntry> InItem, ESelectInfo::Type SelectInfo)
{
	//!< #MY_TODO
}

#undef LOCTEXT_NAMESPACE
