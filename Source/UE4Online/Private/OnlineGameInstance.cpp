// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Online.h"
#include "OnlineGameInstance.h"

#include "OnlineGameSession.h"
#include "OnlineMainMenu.h"

void UOnlineGameInstance::Init()
{
	Super::Init();

	//!< セッション終了時のコールバック
	//OnEndSessionCompleteDelegate = FOnEndSessionCompleteDelegate::CreateUObject(this, &UOnlineGameInstance::OnEndSessionComplete);
}
bool UOnlineGameInstance::JoinSession(ULocalPlayer* LocalPlayer, int32 SessionIndexInSearchResults)
{
	const auto World = GetWorld();
	if (nullptr != World)
	{
		const auto GameMode = World->GetAuthGameMode();
		const auto Session = Cast<AOnlineGameSession>(GameMode->GameSession);
		if (nullptr != Session)
		{
			OnJoinSessionCompleteDelegateHandle = Session->JoinSessionCompleteEvent.AddUObject(this, &UOnlineGameInstance::OnJoinSessionComplete);
			if (Session->JoinSession(LocalPlayer->GetPreferredUniqueNetId(), GameSessionName, SessionIndexInSearchResults))
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("JoinSession"));
				return true;
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
		const auto Session = Cast<AOnlineGameSession>(GameMode->GameSession);
		if (nullptr != Session)
		{
			OnJoinSessionCompleteDelegateHandle = Session->JoinSessionCompleteEvent.AddUObject(this, &UOnlineGameInstance::OnJoinSessionComplete);
			if (Session->JoinSession(LocalPlayer->GetPreferredUniqueNetId(), GameSessionName, SearchResult))
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("JoinSession"));
				return true;
			}
		}
	}

	return false;
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
					UE_LOG(LogUE4Online, Log, TEXT("OnCreateSessionComplete"));
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("OnCreateSettionComplete : ") + Name.ToString());
					//#MY_TODO World->ServerTravel(TravelURL);CreateSession() の引数 InTravelURL を覚えておく
				}
			}
		}
	}
}
void UOnlineGameInstance::OnStartSessionComplete(FName Name, bool bWasSuccessful)
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
				Session->StartSessionCompleteEvent.Remove(OnStartSessionCompleteDelegateHandle);

				if (bWasSuccessful)
				{
					UE_LOG(LogUE4Online, Log, TEXT("OnStartSessionComplete"));
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("OnStartSessionComplete : ") + Name.ToString());
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
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("OnFindSessionComplete : ") + GameSessionName.ToString());
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::FromInt(Session->OnlineSessionSearch->SearchResults.Num()));
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
								GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("OnJoinSessionComplete : ") + URL);

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
					UE_LOG(LogUE4Online, Log, TEXT("OnDestroySessionComplete"));
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("OnDestroySessionComplete : ") + Name.ToString());
				}
			}
		}
	}
}

bool UOnlineGameInstance::CreateSession(ULocalPlayer* LocalPlayer, const FString& InTravelURL)
{
	//!< 非オンラインの場合は ServerTravel() で直接 InTravelURL へ
	if (false)
	{
		const auto World = GetWorld();
		if (nullptr != World)
		{
			World->ServerTravel(InTravelURL);
		}
	}

	//!< InTravelURL から GameType と MapName と bIsLan を作る
	//!< URL の形式 "/Game/Maps/MyMap?game=TDM?listen?bIsLan?Bots=0?DemoRec"
	const FString& MapNameSubStr = "/Game/Maps/";
	//!< RightChop()...指定位置よりも右側の文字列を取得、LeftChop()...指定位置よりも左側の文字列を取得
	const auto ChoppedMapName = InTravelURL.RightChop(MapNameSubStr.Len());
	const auto MapName = ChoppedMapName.LeftChop(ChoppedMapName.Len() - ChoppedMapName.Find("?game"));

	const auto ChoppedGameType = InTravelURL.RightChop(InTravelURL.Find("game=") + FString(TEXT("game=")).Len());
	const auto GameType = ChoppedGameType.LeftChop(ChoppedGameType.Len() - ChoppedGameType.Find("?"));

	const auto bIsLan = InTravelURL.Contains(TEXT("?bIsLan"));

	return CreateSession(LocalPlayer, GameType, MapName, bIsLan);
}
bool UOnlineGameInstance::CreateSession(ULocalPlayer* LocalPlayer, const FString& GameType, const FString& MapName, bool bIsLAN)
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
				//!< CreateSession() が完了した時のデリゲートをイベントへ登録
				OnCreateSessionCompleteDelegateHandle = Session->CreateSessionCompleteEvent.AddUObject(this, &UOnlineGameInstance::OnCreateSessionComplete);

				const auto NumPlayers = 8;
				//!< AOnlineGameSession::CreateSession() をコールする、結果的に CreateSession() がコールされる
				if (Session->CreateSession(LocalPlayer->GetPreferredUniqueNetId(), GameSessionName, GameType, MapName, bIsLAN, true, NumPlayers))
				{
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("CreateSession"));
					return true;
				}
			}
		}
	}

	return false;
}
bool UOnlineGameInstance::StartSession()
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
				//!< StartSession() が完了した時のデリゲートをイベントへ登録
				OnStartSessionCompleteDelegateHandle = Session->StartSessionCompleteEvent.AddUObject(this, &UOnlineGameInstance::OnStartSessionComplete);

				if (Session->StartSession(GameSessionName))
				{
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("StartSession"));
					return true;
				}
			}
		}
	}

	return false;
}
bool UOnlineGameInstance::FindSessions(ULocalPlayer* PlayerOwner, bool bIsLAN)
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

					//!< FindSessions() が完了した時のデリゲートをイベントへ登録
					OnFindSessionsCompleteDelegateHandle = Session->FindSessionsCompleteEvent.AddUObject(this, &UOnlineGameInstance::OnFindSessionsComplete);

					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Finding Sessions ..."));
					Session->FindSessions(PlayerOwner->GetPreferredUniqueNetId(), GameSessionName, bIsLAN, true);
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
				//!< DestroySession() が完了した時のデリゲートをイベントへ登録
				OnDestroySessionCompleteDelegateHandle = Session->DestroySessionCompleteEvent.AddUObject(this, &UOnlineGameInstance::OnDestroySessionComplete);

				if (Session->DestroySession(GameSessionName))
				{
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("DestroySession"));
					return true;
				}
			}
		}
	}

	return false;
}

//!< #MY_TODO UIのテスト用にオーバライドあとで消す
bool UOnlineGameInstance::StartPIEGameInstance(ULocalPlayer* LocalPlayer, bool bInSimulateInEditor, bool bAnyBlueprintErrors, bool bStartInSpectatorMode)
{
	MainMenu = MakeShareable(new FOnlineMainMenu());
	MainMenu->Construct(this, GetFirstGamePlayer());

	if (nullptr != GEngine && nullptr != GEngine->GameViewport)
	{
		//GEngine->GameViewport->AddViewportWidgetContent(MainMenu->MenuWidgetContainer.ToSharedRef());
		//GEngine->GameViewport->AddViewportWidgetContent(MainMenu->CreateSessionWidgetContainer.ToSharedRef());
		//GEngine->GameViewport->AddViewportWidgetContent(MainMenu->FindSessionWidgetContainer.ToSharedRef());
		GEngine->GameViewport->AddViewportWidgetContent(MainMenu->JoinSessionWidgetContainer.ToSharedRef());
	}

	return Super::StartPIEGameInstance(LocalPlayer, bInSimulateInEditor, bAnyBlueprintErrors, bStartInSpectatorMode);
}


