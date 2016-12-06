// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineMainMenu.h"

#include "OnlineMenuWidget.h"
#include "OnlineCreateSessionWidget.h"
#include "OnlineFindSessionWidget.h"
#include "OnlineJoinSessionWidget.h"

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

		SAssignNew(CreateSessionWidget, SOnlineCreateSessionWidget)
			.Cursor(EMouseCursor::Default)
			.LocalPlayer(LocalPlayer.Get());
		SAssignNew(CreateSessionWidgetContainer, SWeakWidget)
			.PossiblyNullContent(CreateSessionWidget);

		SAssignNew(FindSessionWidget, SOnlineFindSessionWidget)
			.Cursor(EMouseCursor::Default)
			.LocalPlayer(LocalPlayer.Get());
		SAssignNew(FindSessionWidgetContainer, SWeakWidget)
			.PossiblyNullContent(FindSessionWidget);

		SAssignNew(JoinSessionWidget, SOnlineJoinSessionWidget)
			.Cursor(EMouseCursor::Default)
			.LocalPlayer(LocalPlayer.Get());
		SAssignNew(JoinSessionWidgetContainer, SWeakWidget)
			.PossiblyNullContent(JoinSessionWidget);
	}
}
