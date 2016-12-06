// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineFindSessionWidget.h"

#define LOCTEXT_NAMESPACE "FindSessionWidget"

void SOnlineFindSessionWidget::Construct(const FArguments& InArgs)
{
	LocalPlayer = InArgs._LocalPlayer;

	//!< �^�C�g��
	const auto TitleTextBlock = SNew(STextBlock)
		.Text(LOCTEXT("FIND_SESSION_TITLE_Key", "Find Session Menu"))
		.ToolTipText(LOCTEXT("FIND_SESSION_TITLE_TIP_Key", "Find Session Menu"));

	//!< IsLAN
	const auto IsLanCheckBox = SNew(SCheckBox)
		.IsChecked(this, &SOnlineFindSessionWidget::IsLanCheckBoxChecked)
		.OnCheckStateChanged(this, &SOnlineFindSessionWidget::OnIsLanCheckBoxStateChanged)
		.ToolTipText(LOCTEXT("ISLAN_TIP_Key", "Is Lan or Internet"))
		[
			SNew(STextBlock)
			.Text(LOCTEXT("ISLAN_Key", "Is Lan"))
		];

	//!< OK, Cancel
	const auto CancelButton = SNew(SButton)
		.Text(LOCTEXT("CANCEL_Key", "Cancel"))
		.ToolTipText(LOCTEXT("CANCEL_TIP_Key", "Cancel"))
		.OnClicked(this, &SOnlineFindSessionWidget::OnCancelButtonClicked);
	const auto OKButton = SNew(SButton)
		.Text(LOCTEXT("OK_Key", "OK"))
		.ToolTipText(LOCTEXT("OK_TIP_Key", "OK"))
		.OnClicked(this, &SOnlineFindSessionWidget::OnOKButtonClicked);
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
			IsLanCheckBox
		];
	VBox->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			OKCancelHBox
		];

	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			VBox
		];
}

ECheckBoxState SOnlineFindSessionWidget::IsLanCheckBoxChecked() const
{
	return bIsLanCheckBoxState ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}
void SOnlineFindSessionWidget::OnIsLanCheckBoxStateChanged(ECheckBoxState CheckBoxState)
{
	bIsLanCheckBoxState = CheckBoxState == ECheckBoxState::Checked ? true : false;
}

FReply SOnlineFindSessionWidget::OnOKButtonClicked()
{
	//!< #MY_TODO
	return FReply::Handled();
}
FReply SOnlineFindSessionWidget::OnCancelButtonClicked()
{
	//!< #MY_TODO
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE

