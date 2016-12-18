// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineJoinSessionWidget.h"

#include "OnlineGameSession.h"
#include "OnlineGameInstance.h"
#include "OnlineMainMenu.h"

#define LOCTEXT_NAMESPACE "JoinSessionWidget"

void SOnlineJoinSessionWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	const auto GameInst = LocalPlayer->GetGameInstance();
	if (nullptr != GameInst)
	{
		const auto World = GameInst->GetWorld();
		if (nullptr != World)
		{
			const auto GameMode = World->GetAuthGameMode();
			if (nullptr != GameMode)
			{
				const auto GameSession = Cast<AOnlineGameSession>(GameMode->GameSession);
				if (nullptr != GameSession)
				{
					const auto SearchState = GameSession->GetOnlineSessionSearch().Get()->SearchState;
					if (PrevSearchState != SearchState)
					{
						switch (SearchState)
						{
						case EOnlineAsyncTaskState::InProgress:
							CircularThrobber->SetVisibility(EVisibility::Visible);
							break; 
						case EOnlineAsyncTaskState::Done:
							CircularThrobber->SetVisibility(EVisibility::Hidden);
							UpdateSearchStatus();
							break;
						default:
						case EOnlineAsyncTaskState::Failed:
						case EOnlineAsyncTaskState::NotStarted:
							CircularThrobber->SetVisibility(EVisibility::Hidden);
							break;
						}
					}
					PrevSearchState = SearchState;
				}
			}
		}
	}	
}

void SOnlineJoinSessionWidget::Construct(const FArguments& InArgs)
{
	LocalPlayer = InArgs._LocalPlayer;

#if 0
	//!< #MY_TODO テスト
	ServerList.Add(MakeShareable(new FServerEntry({ "AAA", "1", "10", "0" })));
	ServerList.Add(MakeShareable(new FServerEntry({ "BBB", "3", "8", "11" })));
	ServerList.Add(MakeShareable(new FServerEntry({ "CCC", "1", "2", "21" })));
	ServerList.Add(MakeShareable(new FServerEntry({ "DDD", "0", "32", "35" })));
	ServerList.Add(MakeShareable(new FServerEntry({ "EEE", "7", "16", "47" })));
	ServerList.Add(MakeShareable(new FServerEntry({ "FFF", "4", "256", "1" })));
	ServerList.Add(MakeShareable(new FServerEntry({ "GGG", "3", "12", "2" })));
	ServerList.Add(MakeShareable(new FServerEntry({ "HHH", "7", "64", "9" })));
	ServerList.Add(MakeShareable(new FServerEntry({ "III", "2", "32", "10" })));
	ServerList.Add(MakeShareable(new FServerEntry({ "JJJ", "1", "16", "44" })));
	ServerList.Add(MakeShareable(new FServerEntry({ "KKK", "3", "48", "78" })));
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
			+ SHeaderRow::Column("GameType").DefaultLabel(LOCTEXT("GameTypeColumn", "Game Type"))
			+ SHeaderRow::Column("Map").DefaultLabel(LOCTEXT("MapColumn", "Map"))
			+ SHeaderRow::Column("PlayerCount").DefaultLabel(LOCTEXT("PlayerCountColumn", "Player Count"))
			+ SHeaderRow::Column("PingInMs").DefaultLabel(LOCTEXT("PingInMsColumn", "PingInMs"))
		);

	//!< 円形スロバー
	CircularThrobber->SetVisibility(EVisibility::Visible);

	//!< Cancel
	const auto CancelButton = SNew(SButton)
		.Text(LOCTEXT("CANCEL_Key", "Cancel"))
		.ToolTipText(LOCTEXT("CANCEL_TIP_Key", "Cancel"))
		.OnClicked(this, &SOnlineJoinSessionWidget::OnCancelButtonClicked);

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
		.VAlign(VAlign_Center)
		[
			Overlay
		];
	VBox->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Bottom)
		[
			CancelButton
		];

	ChildSlot
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SVerticalBox) + SVerticalBox::Slot()
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
			else if (ColumnName == "GameType")
			{
				ItemText = FText::FromString(TEXT("TDM"));
			}
			else if (ColumnName == "Map")
			{
				ItemText = FText::FromString(TEXT("ArenaMap"));
			}
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
	if (EVisibility::Hidden == CircularThrobber->GetVisibility())
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
}
FReply SOnlineJoinSessionWidget::OnCancelButtonClicked()
{
	if (EVisibility::Hidden == CircularThrobber->GetVisibility())
	{
		const auto GameInst = Cast<UOnlineGameInstance>(LocalPlayer->GetGameInstance());
		if (nullptr != GameInst)
		{
			if (nullptr != GEngine && nullptr != GEngine->GameViewport)
			{
				const auto MainMenu = GameInst->GetMainMenu();
				GEngine->GameViewport->RemoveViewportWidgetContent(MainMenu->GetJoinSessionWidgetContainer().ToSharedRef());
				GEngine->GameViewport->AddViewportWidgetContent(MainMenu->GetFindSessionWidgetContainer().ToSharedRef());
			}
		}
	}

	return FReply::Handled();
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

					const auto SessionSearch = Session->GetOnlineSessionSearch();
					if (SessionSearch->SearchResults.Num())
					{
						for (auto i = 0; i<SessionSearch->SearchResults.Num(); ++i)
						{
							const auto& SearchResult = SessionSearch->SearchResults[i];

							const auto MaxPlayerCount = SearchResult.Session.SessionSettings.NumPublicConnections + SearchResult.Session.SessionSettings.NumPrivateConnections;
							const auto AvailabePlayerCount = SearchResult.Session.NumOpenPublicConnections + SearchResult.Session.NumOpenPrivateConnections;
							const auto CurrentPalyerCount = MaxPlayerCount - AvailabePlayerCount;

							auto ServerEntry = MakeShareable(new FServerEntry({ SearchResult.Session.OwningUserName, FString::FromInt(CurrentPalyerCount), FString::FromInt(MaxPlayerCount), FString::FromInt(SearchResult.PingInMs), i }));

							ServerList.Add(ServerEntry);
						}
					}
				}
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
