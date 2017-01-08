// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineCreateSessionWidget.h"

#include "OnlineGameInstance.h"
#include "OnlineMainMenu.h"

#define LOCTEXT_NAMESPACE "CreateSessionWidget"

void SOnlineCreateSessionWidget::Construct(const FArguments& InArgs)
{
	LocalPlayer = InArgs._LocalPlayer;

	//!< タイトル
	const auto TitleTextBlock = SNew(STextBlock)
		.Text(LOCTEXT("CREATE_SESSION_TITLE_Key", "Create Session Menu"))
		.ToolTipText(LOCTEXT("CREATE_SESSION_TITLE_TIP_Key", "Create Session Menu"));

	//!< サーバ名
	const auto ServerNameTextBox = SNew(SEditableTextBox)
		.Text(this, &SOnlineCreateSessionWidget::GetServerNameText)
		.ToolTipText(LOCTEXT("SERVER_NAME_TIP_Key", "Server Name"))
		.OnTextChanged(this, &SOnlineCreateSessionWidget::OnServerNameTextChanged);

	//!< プレイヤ数
	const auto DecreaseButton = SNew(SButton)
		.Text(LOCTEXT("DECREASE_Key", "<"))
		.ToolTipText(LOCTEXT("DECREASE_TIP_Key", "Decrease"))
		.OnClicked(this, &SOnlineCreateSessionWidget::OnDecreasePlayerCountButtonClicked)
		.OnPressed(this, &SOnlineCreateSessionWidget::OnDecreasePlayerCountButtonPressed);
		const auto PlayerCountTextBox = SNew(SEditableTextBox)
		.Text(this, &SOnlineCreateSessionWidget::GetPlayerCountText)
		.ToolTipText(LOCTEXT("PLAYERNUM_TIP_Key", "Player Num"))
		.OnTextChanged(this, &SOnlineCreateSessionWidget::OnPlayerCountTextChanged);
	const auto IncreaseButton = SNew(SButton)
		.Text(LOCTEXT("Increase_Key", ">"))
		.ToolTipText(LOCTEXT("INCREASE_TIP_Key", "Increase"))
		.OnClicked(this, &SOnlineCreateSessionWidget::OnIncreasePlayerCountButtonClicked)
		.OnPressed(this, &SOnlineCreateSessionWidget::OnIncreasePlayerCountButtonPressed);
	const auto PlayerCountHBox = SNew(SHorizontalBox) + SHorizontalBox::Slot();
	PlayerCountHBox->AddSlot()
		.HAlign(HAlign_Left)
		.AutoWidth()
		[
			DecreaseButton
		];
	PlayerCountHBox->AddSlot()
		.HAlign(HAlign_Center)
		.AutoWidth()
		[
			PlayerCountTextBox
		];
	PlayerCountHBox->AddSlot()
		.HAlign(HAlign_Right)
		.AutoWidth()
		[
			IncreaseButton
		];
	PlayerCountHBox->SetEnabled(false); //!< #MY_TODO 現状8固定

	//!< LAN or インターネット
	const auto IsLanCheckBox = SNew(SCheckBox)
		.IsChecked(this, &SOnlineCreateSessionWidget::IsLanCheckBoxChecked)
		.OnCheckStateChanged(this, &SOnlineCreateSessionWidget::OnIsLanCheckBoxStateChanged)
		.ToolTipText(LOCTEXT("ISLAN_TIP_Key", "Is Lan or Internet"))
		[
			SNew(STextBlock)
			.Text(LOCTEXT("ISLAN_Key", "Is Lan"))
		];

	//!< OK, Cancel
	const auto CancelButton = SNew(SButton)
		.Text(LOCTEXT("CANCEL_Key", "Cancel"))
		.ToolTipText(LOCTEXT("CANCEL_TIP_Key", "Cancel"))
		.OnClicked(this, &SOnlineCreateSessionWidget::OnCancelButtonClicked)
		.OnPressed(this, &SOnlineCreateSessionWidget::OnCancelButtonPressed);
	const auto OKButton = SNew(SButton)
		.Text(LOCTEXT("OK_Key", "OK"))
		.ToolTipText(LOCTEXT("OK_TIP_Key", "OK"))
		.OnClicked(this, &SOnlineCreateSessionWidget::OnOKButtonClicked)
		.OnPressed(this, &SOnlineCreateSessionWidget::OnOKButtonPressed);
		const auto OKCancelHBox = SNew(SHorizontalBox) + SHorizontalBox::Slot();
	OKCancelHBox->AddSlot()
		.HAlign(HAlign_Left)
		.AutoWidth()
		[
			CancelButton
		];
	OKCancelHBox->AddSlot()
		.HAlign(HAlign_Right)
		.AutoWidth()
		[
			OKButton
		];

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
			ServerNameTextBox
		];
	VBox->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			PlayerCountHBox
		];
	VBox->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			IsLanCheckBox
		];
	VBox->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Bottom)
		[
			OKCancelHBox
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
				.WidthOverride(600)
				.HeightOverride(300)
				[
					VBox
				]
			]
		];
}

FText  SOnlineCreateSessionWidget::GetServerNameText() const
{
	return FText::FromString(ServerName);
}
void SOnlineCreateSessionWidget::OnServerNameTextChanged(const FText& InLabel)
{
	ServerName = InLabel.ToString();
}

FText SOnlineCreateSessionWidget::GetPlayerCountText() const
{
	return FText::FromString(FString::FromInt(PlayerCount));
}
void SOnlineCreateSessionWidget::OnPlayerCountTextChanged(const FText& InLabel)
{
	PlayerCount = FCString::Atoi(*InLabel.ToString());
}
FReply SOnlineCreateSessionWidget::OnDecreasePlayerCountButtonClicked()
{
	PlayerCount = FMath::Max(PlayerCount - 1, 1);
	return FReply::Handled();
}
FReply SOnlineCreateSessionWidget::OnIncreasePlayerCountButtonClicked()
{
	PlayerCount = FMath::Min(PlayerCount + 1, 8);
	return FReply::Handled();
}

ECheckBoxState SOnlineCreateSessionWidget::IsLanCheckBoxChecked() const
{
	return bIsLanCheckBoxState ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}
void SOnlineCreateSessionWidget::OnIsLanCheckBoxStateChanged(ECheckBoxState CheckBoxState)
{
	bIsLanCheckBoxState = CheckBoxState == ECheckBoxState::Checked ? true : false;
}

FReply SOnlineCreateSessionWidget::OnOKButtonClicked()
{
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveAllViewportWidgets();
		FSlateApplication::Get().SetUserFocusToGameViewport(FSlateApplication::Get().GetUserIndexForKeyboard());
	}
	
	const auto GameInst = Cast<UOnlineGameInstance>(LocalPlayer->GetGameInstance());
	if (nullptr != GameInst)
	{
		const auto MapName = FString(TEXT("ArenaMap")); //!< #MY_TODO 現状固定
		const auto GameType = FString(TEXT("TDM"));		//!< #MY_TODO 現状固定
		const auto IsOnline = FString(true ? TEXT("?listen") : TEXT(""));
		const auto IsLanMatch = FString(bIsLanCheckBoxState ? TEXT("?bIsLanMatch") : TEXT(""));
		const auto MaxNumPlayers = FString::FromInt(PlayerCount); //!< #MY_TODO 現状固定

		const auto InTravelURL = FString::Printf(TEXT("/Game/Online/Map/%s?game=%s%s%s"), *MapName, *GameType, *IsOnline, *IsLanMatch);
		GameInst->CreateSession(LocalPlayer.Get(), InTravelURL);
	}

	return FReply::Handled();
}
FReply SOnlineCreateSessionWidget::OnCancelButtonClicked()
{
	const auto GameInst = Cast<UOnlineGameInstance>(LocalPlayer->GetGameInstance());
	if (nullptr != GameInst)
	{
		if (nullptr != GEngine && nullptr != GEngine->GameViewport)
		{
			const auto MainMenu = GameInst->GetMainMenu();
			const auto UserIndex = FSlateApplication::Get().GetUserIndexForKeyboard();

			GEngine->GameViewport->RemoveViewportWidgetContent(MainMenu->GetCreateSessionWidgetContainer().ToSharedRef());
			FSlateApplication::Get().SetUserFocusToGameViewport(UserIndex);

			GEngine->GameViewport->AddViewportWidgetContent(MainMenu->GetMenuWidgetContainer().ToSharedRef());
			FSlateApplication::Get().SetUserFocus(UserIndex, MainMenu->GetMenuWidgetContainer().ToSharedRef());
		}
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE

