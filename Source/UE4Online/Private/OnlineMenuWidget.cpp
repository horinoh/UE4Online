// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineMenuWidget.h"

#define LOCTEXT_NAMESPACE "Namespace"

void SOnlineMenuWidget::Construct(const FArguments& InArgs)
{
	const auto TextBlock = SNew(STextBlock)
		.Text(LOCTEXT("STextBlock_Key", "STextBlock_Text"))
		//.Font(FEditorStyle::GetFontStyle(FName("ToolBarButton.LabelFont")))
		.ToolTipText(LOCTEXT("STextBlock_Key", "STextBlock_Text"));
	const auto Button = SNew(SButton)
		.Text(LOCTEXT("SButton_Key", "SButton_Text"))
		.ToolTipText(LOCTEXT("SButton_Key", "SButton_Text"))
		;// .OnClicked(this, &SHUDWidget::OnButtonClicked);

	ChildSlot
	[
		SNew(SScrollBox)
		+ SScrollBox::Slot()
		.Padding(10, 5)
		[
			Button
		]
	];
}

#undef LOCTEXT_NAMESPACE
