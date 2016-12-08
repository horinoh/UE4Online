// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineJoinSessionWidget.h"

#include "SThrobber.h"

#include "OnlineGameSession.h"

#define LOCTEXT_NAMESPACE "JoinSessionWidget"

void SOnlineJoinSessionWidget::Construct(const FArguments& InArgs)
{
	LocalPlayer = InArgs._LocalPlayer;

#if 1
	//!< #MY_TODO テスト
	ServerList.Add(MakeShareable(new FServerEntry({ "AAA", "1", "10", "0" })));
	ServerList.Add(MakeShareable(new FServerEntry({ "BBB", "3", "8", "11" })));
	ServerList.Add(MakeShareable(new FServerEntry({ "CCC", "1", "2", "21" })));
	ServerList.Add(MakeShareable(new FServerEntry({ "DDD", "0", "32", "35" })));
	ServerList.Add(MakeShareable(new FServerEntry({ "EEE", "7", "16", "47" })));
	ServerList.Add(MakeShareable(new FServerEntry({ "FFF", "4", "256", "1" })));
#endif

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
		//.OnSelectionChanged(this, &SOnlineJoinSessionWidget::OnServerEntrySelectionChanged)
		.OnMouseButtonDoubleClick(this, &SOnlineJoinSessionWidget::OnServerEntryMouseButtonDoubleClicked)
		.HeaderRow(
			SNew(SHeaderRow)
			+ SHeaderRow::Column("ServerName").FixedWidth(100).DefaultLabel(LOCTEXT("ServerNameColumn", "Server Name"))
			//+ SHeaderRow::Column("GameType").DefaultLabel(LOCTEXT("GameTypeColumn", "Game Type"))
			//+ SHeaderRow::Column("Map").DefaultLabel(LOCTEXT("MapColumn", "Map"))
			+ SHeaderRow::Column("PlayerCount").DefaultLabel(LOCTEXT("PlayerCountColumn", "Player Count"))
			+ SHeaderRow::Column("PingInMs").DefaultLabel(LOCTEXT("PingInMsColumn", "PingInMs"))
		);

	//!< ローディング中表示
	const auto CircularThrobber = SNew(SCircularThrobber);
	//CircularThrobber->SetVisibility(EVisibility::Hidden);
	CircularThrobber->SetVisibility(EVisibility::Visible);

	//!< リストビューとローディング中は重ねて表示するので SOverlay へ含める
	const auto Overlay = SNew(SOverlay);
	Overlay->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			ServerListView
		];
	Overlay->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			CircularThrobber
		];

	const auto VBox = SNew(SVerticalBox);
	VBox->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Top)
		[
			TitleTextBlock
		];
	VBox->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Bottom)
		[
			Overlay
		];

	ChildSlot
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBox)
				.WidthOverride(300)
				.HeightOverride(300)
				[
					VBox
				]
			]
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
			if (ColumnName == "ServerName")
			{
				ItemText = FText::FromString(Item->ServerName);
			}
			//else if (ColumnName == "GameType")
			//{
			//}
			//else if (ColumnName == "Map")
			//{
			//}
			else if (ColumnName == "PlayerCount")
			{
				ItemText = FText::Format(FText::FromString("{0} / {1}"), FText::FromString(Item->CurrentPlayerCount), FText::FromString(Item->MaxPlayerCount));
			}
			else if (ColumnName == "PingInMs")
			{
				ItemText = FText::FromString(Item->PingInMs);
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
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveAllViewportWidgets();
	}
	const auto GameInst = LocalPlayer->GetGameInstance();
	if (nullptr != GameInst)
	{
		GameInst->JoinSession(LocalPlayer.Get(), InItem->Index);
	}
}

void SOnlineJoinSessionWidget::UpdateSearchStatus()
{
	const auto GameInst = LocalPlayer->GetGameInstance();
	if (nullptr != GameInst)
	{
		const auto World = GameInst->GetWorld();
		if (nullptr != World)
		{
			const auto GameMode = World->GetAuthGameMode();
			if (nullptr != GameMode)
			{
				const auto Session = Cast<AOnlineGameSession>(GameMode->GameSession);
				if (nullptr != Session)
				{
					ServerList.Empty();

					const auto SessionSearch = Session->OnlineSessionSearch;
					if (SessionSearch->SearchResults.Num())
					{
						for (auto i = 0; i<SessionSearch->SearchResults.Num(); ++i)
						{
							const auto& SR = SessionSearch->SearchResults[i];

							const auto MaxPlayerCount = SR.Session.SessionSettings.NumPublicConnections + SR.Session.SessionSettings.NumPrivateConnections;
							const auto AvailabePlayerCount = SR.Session.NumOpenPublicConnections + SR.Session.NumOpenPrivateConnections;
							const auto CurrentPalyerCount = MaxPlayerCount - AvailabePlayerCount;

							auto ServerEntry = MakeShareable(new FServerEntry({ SR.Session.OwningUserName, FString::FromInt(CurrentPalyerCount), FString::FromInt(MaxPlayerCount), FString::FromInt(SR.PingInMs), i }));

							ServerList.Add(ServerEntry);
						}
					}
				}
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
