// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineGameInstance.h"

#include "OnlineGameSession.h"
#include "OnlineMainMenu.h"

void UOnlineGameInstance::Init()
{
	Super::Init();

	//!< #MY_TODO callbacks

	TickDelegate = FTickerDelegate::CreateUObject(this, &UOnlineGameInstance::Tick);
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);
}
void UOnlineGameInstance::Shutdown()
{
	Super::Shutdown();

	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
}

void UOnlineGameInstance::StartGameInstance()
{
	Super::StartGameInstance();

	MainMenu = MakeShareable(new FOnlineMainMenu());
	MainMenu->Construct(this, GetFirstGamePlayer());
	if (nullptr != GEngine && nullptr != GEngine->GameViewport)
	{
		const auto UserIndex = FSlateApplication::Get().GetUserIndexForKeyboard();

		GEngine->GameViewport->AddViewportWidgetContent(MainMenu->GetMenuWidgetContainer().ToSharedRef());
		FSlateApplication::Get().SetUserFocus(UserIndex, MainMenu->GetMenuWidgetContainer().ToSharedRef());
	}
}
//bool UOnlineGameInstance::StartPIEGameInstance(ULocalPlayer* LocalPlayer, bool bInSimulateInEditor, bool bAnyBlueprintErrors, bool bStartInSpectatorMode)
//{
//	MainMenu = MakeShareable(new FOnlineMainMenu());
//	MainMenu->Construct(this, GetFirstGamePlayer());
//	if (nullptr != GEngine && nullptr != GEngine->GameViewport)
//	{
//		const auto UserIndex = FSlateApplication::Get().GetUserIndexForKeyboard();
//
//		GEngine->GameViewport->AddViewportWidgetContent(MainMenu->GetMenuWidgetContainer().ToSharedRef());
//		FSlateApplication::Get().SetUserFocus(UserIndex, MainMenu->GetMenuWidgetContainer().ToSharedRef());
//	}
//
//	return Super::StartPIEGameInstance(LocalPlayer, bInSimulateInEditor, bAnyBlueprintErrors, bStartInSpectatorMode);
//}
FGameInstancePIEResult UOnlineGameInstance::StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer, const FGameInstancePIEParameters& Params)
{
	MainMenu = MakeShareable(new FOnlineMainMenu());
	MainMenu->Construct(this, GetFirstGamePlayer());
	if (nullptr != GEngine && nullptr != GEngine->GameViewport)
	{
		const auto UserIndex = FSlateApplication::Get().GetUserIndexForKeyboard();

		GEngine->GameViewport->AddViewportWidgetContent(MainMenu->GetMenuWidgetContainer().ToSharedRef());
		FSlateApplication::Get().SetUserFocus(UserIndex, MainMenu->GetMenuWidgetContainer().ToSharedRef());
	}

	return Super::StartPlayInEditorGameInstance(LocalPlayer, Params);
}

bool UOnlineGameInstance::JoinSession(ULocalPlayer* LocalPlayer, int32 SessionIndexInSearchResults)
{
	const auto World = GetWorld();
	if (nullptr != World)
	{
		const auto GameMode = World->GetAuthGameMode();
		if (nullptr != GameMode)
		{
			const auto Session = Cast<AOnlineGameSession>(GameMode->GameSession);
			if (nullptr != Session)
			{
				OnJoinSessionCompleteDelegateHandle = Session->JoinSessionCompleteEvent.AddUObject(this, &UOnlineGameInstance::OnJoinSessionComplete);
				if (Session->JoinSession(LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId(), GameSessionName, SessionIndexInSearchResults))
				{
					return true;
				}
			}
		}
	}

	return false;
}
bool UOnlineGameInstance::JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult)
{
	const auto World = GetWorld();
	if (nullptr != World)
	{
		const auto GameMode = World->GetAuthGameMode();
		if (nullptr != GameMode)
		{
			const auto Session = Cast<AOnlineGameSession>(GameMode->GameSession);
			if (nullptr != Session)
			{
				OnJoinSessionCompleteDelegateHandle = Session->JoinSessionCompleteEvent.AddUObject(this, &UOnlineGameInstance::OnJoinSessionComplete);
				if (Session->JoinSession(LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId(), GameSessionName, SearchResult))
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool UOnlineGameInstance::Tick(float DeltaSeconds)
{
	if (IsRunningDedicatedServer())
	{
		return true;
	}

	const auto OnlineSub = IOnlineSubsystem::Get();
	if (nullptr != OnlineSub)
	{
		const auto Session = OnlineSub->GetSessionInterface();
		if (Session.IsValid())
		{
		 	const auto SessionState = Session->GetSessionState(GameSessionName);
			const auto SessionStateString = EOnlineSessionState::ToString(SessionState);
			//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, SessionStateString);
		}
	}
	return true;
}

void UOnlineGameInstance::OnCreateSessionComplete(FName Name, bool bWasSuccessful)
{
	const auto World = GetWorld();
	if (nullptr != World)
	{
		const auto GameMode = World->GetAuthGameMode();
		if (nullptr != GameMode)
		{
			const auto Session = Cast<AOnlineGameSession>(GameMode->GameSession);
			if (nullptr != Session)
			{
				Session->CreateSessionCompleteEvent.Remove(OnCreateSessionCompleteDelegateHandle);

				if (bWasSuccessful)
				{
					World->ServerTravel(TravelURL);
				}
			}
		}
	}
}
void UOnlineGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	const auto World = GetWorld();
	if (nullptr != World)
	{
		const auto GameMode = World->GetAuthGameMode();
		if (nullptr != GameMode)
		{
			const auto Session = Cast<AOnlineGameSession>(GameMode->GameSession);
			if (nullptr != Session)
			{
				Session->FindSessionsCompleteEvent.Remove(OnFindSessionsCompleteDelegateHandle);

				if (bWasSuccessful)
				{
				}
			}
		}
	}
}
void UOnlineGameInstance::OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result)
{
	const auto World = GetWorld();
	if (nullptr != World)
	{
		const auto GameMode = World->GetAuthGameMode();
		if (nullptr != GameMode)
		{
			const auto Session = Cast<AOnlineGameSession>(GameMode->GameSession);
			if (nullptr != Session)
			{
				Session->JoinSessionCompleteEvent.Remove(OnJoinSessionCompleteDelegateHandle);

				switch (Result)
				{
				case EOnJoinSessionCompleteResult::Success:
				{
					const auto OnlineSub = IOnlineSubsystem::Get();
					if (nullptr != OnlineSub)
					{
						const auto SessionInterface = OnlineSub->GetSessionInterface();
						if (SessionInterface.IsValid())
						{
							FString URL;
							if (SessionInterface->GetResolvedConnectString(GameSessionName, URL))
							{
								const auto PC = GetFirstLocalPlayerController();
								if (nullptr != PC)
								{
									PC->ClientTravel(URL, TRAVEL_Absolute);
								}
							}
						}
					}
				}
				break;
				case EOnJoinSessionCompleteResult::SessionIsFull:
					break;
				case EOnJoinSessionCompleteResult::SessionDoesNotExist:
					break;
				default:
				case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
				case EOnJoinSessionCompleteResult::AlreadyInSession:
				case EOnJoinSessionCompleteResult::UnknownError:
					break;
				}
			}
		}
	}
}
void UOnlineGameInstance::OnDestroySessionComplete(FName Name, bool bWasSuccessful)
{
	const auto World = GetWorld();
	if (nullptr != World)
	{
		const auto GameMode = World->GetAuthGameMode();
		if (nullptr != GameMode)
		{
			const auto Session = Cast<AOnlineGameSession>(GameMode->GameSession);
			if (nullptr != Session)
			{
				Session->DestroySessionCompleteEvent.Remove(OnDestroySessionCompleteDelegateHandle);

				if (bWasSuccessful)
				{
				}
			}
		}
	}
}
bool UOnlineGameInstance::CreateSession(ULocalPlayer* LocalPlayer, const FString& InTravelURL)
{
	TravelURL = InTravelURL;

	//!< If not online direct to ServerTravel(InTravelURL)
	if (false)
	{
		const auto World = GetWorld();
		if (nullptr != World)
		{
			World->ServerTravel(TravelURL);
		}
	}

	//!< Get MapName, GameType, IsLanMatch form TravelURL
	//!< URL format : "/Game/Online/Map/MAPNAME?game=GAMETYPE?listen?bIsLanMatch"
	const FString& MapNameSubStr = "/Game/Online/Map/";

	const auto ChoppedMapName = TravelURL.RightChop(MapNameSubStr.Len());
	const auto MapName = ChoppedMapName.LeftChop(ChoppedMapName.Len() - ChoppedMapName.Find("?game"));

	const auto ChoppedGameType = TravelURL.RightChop(TravelURL.Find("game=") + FString(TEXT("game=")).Len());
	const auto GameType = ChoppedGameType.LeftChop(ChoppedGameType.Len() - ChoppedGameType.Find("?"));

	const auto bIsLanMatch = TravelURL.Contains(TEXT("?bIsLanMatch"));

	const auto MaxNumPlayers = 8; //!< #MY_TODO 

	return CreateSession(LocalPlayer, GameType, MapName, bIsLanMatch, MaxNumPlayers);
}
bool UOnlineGameInstance::CreateSession(ULocalPlayer* LocalPlayer, const FString& GameType, const FString& MapName, bool bIsLanMatch, int32 MaxNumPlayers)
{
	const auto World = GetWorld();
	if (nullptr != World)
	{
		const auto GameMode = World->GetAuthGameMode();
		if (nullptr != GameMode)
		{
			const auto Session = Cast<AOnlineGameSession>(GameMode->GameSession);
			if (nullptr != Session)
			{
				OnCreateSessionCompleteDelegateHandle = Session->CreateSessionCompleteEvent.AddUObject(this, &UOnlineGameInstance::OnCreateSessionComplete);

				const auto bIsPresence = true;
				if (Session->CreateSession(LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId(), GameSessionName, GameType, MapName, bIsLanMatch, bIsPresence, MaxNumPlayers))
				{
					return true;
				}
			}
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, (TEXT("CreateSession [%s] Failed"), GameSessionName.ToString()));
	return false;
}
bool UOnlineGameInstance::FindSessions(ULocalPlayer* PlayerOwner, bool bIsLanMatch)
{
	if (nullptr != PlayerOwner)
	{
		const auto World = GetWorld();
		if (nullptr != World)
		{
			const auto GameMode = World->GetAuthGameMode();
			if (nullptr != GameMode)
			{
				const auto Session = Cast<AOnlineGameSession>(GameMode->GameSession);
				if (nullptr != Session)
				{
					Session->FindSessionsCompleteEvent.RemoveAll(this);
					OnFindSessionsCompleteDelegateHandle = Session->FindSessionsCompleteEvent.AddUObject(this, &UOnlineGameInstance::OnFindSessionsComplete);

					Session->FindSessions(PlayerOwner->GetPreferredUniqueNetId().GetUniqueNetId(), GameSessionName, bIsLanMatch, true);
					return true;
				}
			}
		}
	}

	return false;
}

bool UOnlineGameInstance::DestroySession()
{
	const auto World = GetWorld();
	if (nullptr != World)
	{
		const auto GameMode = World->GetAuthGameMode();
		if (nullptr != GameMode)
		{
			const auto Session = Cast<AOnlineGameSession>(GameMode->GameSession);
			if (nullptr != Session)
			{
				OnDestroySessionCompleteDelegateHandle = Session->DestroySessionCompleteEvent.AddUObject(this, &UOnlineGameInstance::OnDestroySessionComplete);

				if (Session->DestroySession(GameSessionName))
				{
					return true;
				}
			}
		}
	}

	return false;
}
