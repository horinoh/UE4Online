// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineMenuWidget.h"

#include "OnlineGameInstance.h"
#include "OnlineMainMenu.h"

#define LOCTEXT_NAMESPACE "MenuWidget"

void SOnlineMenuWidget::Construct(const FArguments& InArgs)
{
	LocalPlayer = InArgs._LocalPlayer;

	//!< タイトル
	const auto TitleTextBlock = SNew(STextBlock)
		.Text(LOCTEXT("MAIN_TITLE_Key", "Main Menu"))
		//.Font(FEditorStyle::GetFontStyle(FName("ToolBarButton.LabelFont")))
		.ToolTipText(LOCTEXT("MAIN_TITLE_TIP_Key", "Main Menu"));

	//!< セッション作成
	const auto CreateButton = SNew(SButton)
		.Text(LOCTEXT("CREATE_SESSION_Key", "Create Session"))
		.ToolTipText(LOCTEXT("CREATE_SESSION_TIP_Key", "Create Session"))
		.OnClicked(this, &SOnlineMenuWidget::OnCreateSessionButtonClicked);

	//!< セッション検索
	const auto FindButton = SNew(SButton)
		.Text(LOCTEXT("FIND_SESSION_Key", "Find Session"))
		.ToolTipText(LOCTEXT("FIND_SESSION_TIP_Key", "Find Session"))
		.OnClicked(this, &SOnlineMenuWidget::OnFindSessionButtonClicked);

	//!< 終了
	const auto QuitButton = SNew(SButton)
		.Text(LOCTEXT("QUIT_Key", "Quit"))
		.ToolTipText(LOCTEXT("QUIT_TIP_Key", "Quit"))
		.OnClicked(this, &SOnlineMenuWidget::OnQuitButtonClicked);

	const auto VBox = SNew(SVerticalBox) + SVerticalBox::Slot();
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
			CreateButton
		];
	VBox->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			FindButton
		];
	VBox->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Bottom)
		[
			QuitButton
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

FReply SOnlineMenuWidget::OnCreateSessionButtonClicked()
{
	const auto GameInst = Cast<UOnlineGameInstance>(LocalPlayer->GetGameInstance());
	if (nullptr != GameInst)
	{
		if (nullptr != GEngine && nullptr != GEngine->GameViewport)
		{
			const auto MainMenu = GameInst->GetMainMenu();
			GEngine->GameViewport->RemoveViewportWidgetContent(MainMenu->GetMenuWidgetContainer().ToSharedRef());
			GEngine->GameViewport->AddViewportWidgetContent(MainMenu->GetCreateSessionWidgetContainer().ToSharedRef());
		}
	}

	return FReply::Handled();
}
FReply SOnlineMenuWidget::OnFindSessionButtonClicked()
{
	const auto GameInst = Cast<UOnlineGameInstance>(LocalPlayer->GetGameInstance());
	if (nullptr != GameInst)
	{
		if (nullptr != GEngine && nullptr != GEngine->GameViewport)
		{
			const auto MainMenu = GameInst->GetMainMenu();
			GEngine->GameViewport->RemoveViewportWidgetContent(MainMenu->GetMenuWidgetContainer().ToSharedRef());
			GEngine->GameViewport->AddViewportWidgetContent(MainMenu->GetFindSessionWidgetContainer().ToSharedRef());
		}
	}

	return FReply::Handled();
}
FReply SOnlineMenuWidget::OnQuitButtonClicked()
{
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveAllViewportWidgets();
	}

	//!< #MY_TODO
	LocalPlayer->GetGameInstance()->GetGameViewportClient()->ConsoleCommand("quit");

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
