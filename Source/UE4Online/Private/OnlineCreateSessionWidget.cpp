// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineCreateSessionWidget.h"

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
		.OnClicked(this, &SOnlineCreateSessionWidget::OnDecreasePlayerCountButtonClicked);
	const auto PlayerCountTextBox = SNew(SEditableTextBox)
		.Text(this, &SOnlineCreateSessionWidget::GetPlayerCountText)
		.ToolTipText(LOCTEXT("PLAYERNUM_TIP_Key", "Player Num"))
		.OnTextChanged(this, &SOnlineCreateSessionWidget::OnPlayerCountTextChanged);
	const auto IncreaseButton = SNew(SButton)
		.Text(LOCTEXT("Increase_Key", ">"))
		.ToolTipText(LOCTEXT("INCREASE_TIP_Key", "Increase"))
		.OnClicked(this, &SOnlineCreateSessionWidget::OnIncreasePlayerCountButtonClicked);
	const auto PlayerCountHBox = SNew(SHorizontalBox) + SHorizontalBox::Slot();
	PlayerCountHBox->AddSlot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		[
			DecreaseButton
		];
	PlayerCountHBox->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			PlayerCountTextBox
		];
	PlayerCountHBox->AddSlot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Center)
		[
			IncreaseButton
		];

	//!< IsLAN
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
		.OnClicked(this, &SOnlineCreateSessionWidget::OnCancelButtonClicked);
	const auto OKButton = SNew(SButton)
		.Text(LOCTEXT("OK_Key", "OK"))
		.ToolTipText(LOCTEXT("OK_TIP_Key", "OK"))
		.OnClicked(this, &SOnlineCreateSessionWidget::OnOKButtonClicked);
	const auto OKCancelHBox = SNew(SHorizontalBox) + SHorizontalBox::Slot();
	OKCancelHBox->AddSlot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		[
			CancelButton
		];
	OKCancelHBox->AddSlot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Center)
		[
			OKButton
		];

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
		.VAlign(VAlign_Center)
		[
			OKCancelHBox
		];

	ChildSlot
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			VBox
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
	//!< #MY_TODO
	return FReply::Handled();
}
FReply SOnlineCreateSessionWidget::OnCancelButtonClicked()
{
	//!< #MY_TODO
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE

