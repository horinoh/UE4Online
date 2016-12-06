// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineMenuWidget.h"

#define LOCTEXT_NAMESPACE "MenuWidget"

void SOnlineMenuWidget::Construct(const FArguments& InArgs)
{
	LocalPlayer = InArgs._LocalPlayer;

	//!< ƒ^ƒCƒgƒ‹
	const auto TitleTextBlock = SNew(STextBlock)
		.Text(LOCTEXT("MAIN_TITLE_Key", "Main Menu"))
		//.Font(FEditorStyle::GetFontStyle(FName("ToolBarButton.LabelFont")))
		.ToolTipText(LOCTEXT("MAIN_TITLE_TIP_Key", "Main Menu"));

	const auto CreateButton = SNew(SButton)
		.Text(LOCTEXT("CREATE_SESSION_Key", "Create Session"))
		.ToolTipText(LOCTEXT("CREATE_SESSION_TIP_Key", "Create Session"))
		.OnClicked(this, &SOnlineMenuWidget::OnCreateSessionButtonClicked);

	const auto FindButton = SNew(SButton)
		.Text(LOCTEXT("FIND_SESSION_Key", "Find Session"))
		.ToolTipText(LOCTEXT("FIND_SESSION_TIP_Key", "Find Session"))
		.OnClicked(this, &SOnlineMenuWidget::OnJoinSessionButtonClicked);

	const auto QuitButton = SNew(SButton)
		.Text(LOCTEXT("QUIT_Key", "Quit"))
		.ToolTipText(LOCTEXT("QUIT_TIP_Key", "Quit"))
		.OnClicked(this, &SOnlineMenuWidget::OnQuitButtonClicked);

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
		.VAlign(VAlign_Center)
		[
			QuitButton
		];

	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			VBox
		];
}

FReply SOnlineMenuWidget::OnCreateSessionButtonClicked()
{
	//!< #MY_TODO
	return FReply::Handled();
}
FReply SOnlineMenuWidget::OnJoinSessionButtonClicked()
{
	//!< #MY_TODO
	return FReply::Handled();
}
FReply SOnlineMenuWidget::OnQuitButtonClicked()
{
	//!< #MY_TODO
	//	LocalPlayer->GetGameInstance()->GetGameViewportClient()->ConsoleCommand("quit");
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
