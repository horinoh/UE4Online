// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineMainMenu.h"

#include "OnlineMenuWidget.h"

void FOnlineMainMenu::Tick(float DeltaTime)
{
}
TStatId FOnlineMainMenu::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FOnlineMainMenu, STATGROUP_Tickables);
}

void FOnlineMainMenu::Construct(TWeakObjectPtr<class UOnlineGameInstance> GI, TWeakObjectPtr<ULocalPlayer> LP)
{
	GameInstance = GI;
	LocalPlayer = LP;

	{
		SAssignNew(MenuWidget, SOnlineMenuWidget)
			.Cursor(EMouseCursor::Default)
			.LocalPlayer(LocalPlayer.Get());

		SAssignNew(MenuWidgetContainer, SWeakWidget)
			.PossiblyNullContent(MenuWidget);
	}
}
