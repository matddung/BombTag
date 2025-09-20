#include "BombTagGameInstance.h"
#include "BombTag.h"
#include "BombTagSaveGame.h"

#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Engine/Engine.h"

namespace
{
    constexpr int32 PlayerSaveSlotIndex = 0;
    const TCHAR* PlayerSaveSlotName = TEXT("PlayerProfile");
    const FName BombTagSessionName = NAME_GameSession;
    const FName SessionDisplayNameKey(TEXT("SERVER_NAME"));
    const FName SessionPasswordKey(TEXT("SERVER_PASSWORD"));
}

void UBombTagGameInstance::Init()
{
    Super::Init();

    LoadOrCreatePlayerData();

    UE_LOG(LogBombTag, Log, TEXT("BombTag GameInstance initialized"));

    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IdentityInterface = Subsystem->GetIdentityInterface();
        SessionInterface = Subsystem->GetSessionInterface();

        if (!SessionInterface.IsValid())
        {
            UE_LOG(LogBombTag, Warning, TEXT("Session interface not found"));
        }

        if (IdentityInterface.IsValid())
        {
            LoginCompleteDelegateHandle = IdentityInterface->AddOnLoginCompleteDelegate_Handle(0,
                FOnLoginCompleteDelegate::CreateUObject(this, &UBombTagGameInstance::HandleLoginComplete));

            if (!IdentityInterface->AutoLogin(0))
            {
                UE_LOG(LogBombTag, Log, TEXT("AutoLogin failed to start, trying deviceid login"));
                AttemptDeviceIdLogin();
            }
        }
        else
        {
            UE_LOG(LogBombTag, Warning, TEXT("Identity interface not found"));
        }
    }
    else
    {
        UE_LOG(LogBombTag, Warning, TEXT("OnlineSubsystem not found"));
    }
}

void UBombTagGameInstance::AttemptDeviceIdLogin()
{
    if (IdentityInterface.IsValid() && !bAttemptedDeviceIdLogin)
    {
        bAttemptedDeviceIdLogin = true;
        FOnlineAccountCredentials Credentials;
        Credentials.Type = TEXT("deviceid");
        Credentials.Id = TEXT("");
        Credentials.Token = TEXT("");
        IdentityInterface->Login(0, Credentials);
    }
}

void UBombTagGameInstance::HandleLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
    if (bWasSuccessful)
    {
        UE_LOG(LogBombTag, Log, TEXT("Login succeeded: %s"), *UserId.ToString());
    }
    else
    {
        UE_LOG(LogBombTag, Error, TEXT("Login failed: %s"), *Error);
        if (!bAttemptedDeviceIdLogin)
        {
            UE_LOG(LogBombTag, Log, TEXT("Attempting deviceid login"));
            AttemptDeviceIdLogin();
            return;
        }
    }

    if (IdentityInterface.IsValid())
    {
        IdentityInterface->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, LoginCompleteDelegateHandle);
    }
}

void UBombTagGameInstance::HostOnlineSession(const FString& SessionName, const FString& SessionPassword, int32 MaxPublicConnections, bool bIsLanMatch)
{
    if (!SessionInterface.IsValid())
    {
        UE_LOG(LogBombTag, Warning, TEXT("Cannot host session because session interface is invalid."));
        return;
    }

    PendingHostSessionName = SessionName;
    PendingHostSessionName.TrimStartAndEndInline();
    if (PendingHostSessionName.IsEmpty())
    {
        PendingHostSessionName = TEXT("BombTag Session");
    }

    PendingHostSessionPassword = SessionPassword;
    PendingHostSessionPassword.TrimStartAndEndInline();

    PendingHostPublicConnections = FMath::Clamp(MaxPublicConnections, 1, 64);
    bPendingHostLanMatch = bIsLanMatch;

    if (SessionInterface->GetNamedSession(BombTagSessionName) != nullptr)
    {
        bCreateSessionAfterDestroy = true;
        bReturnToMenuAfterDestroy = false;
        DestroySessionInternal();
    }
    else
    {
        CreateSessionInternal();
    }
}

void UBombTagGameInstance::FindAndJoinSession(const FString& SessionName, const FString& SessionPassword, bool bIsLanQuery)
{
    if (!SessionInterface.IsValid())
    {
        UE_LOG(LogBombTag, Warning, TEXT("Cannot search for sessions because session interface is invalid."));
        return;
    }

    PendingJoinSessionName = SessionName;
    PendingJoinSessionName.TrimStartAndEndInline();
    PendingJoinSessionPassword = SessionPassword;
    PendingJoinSessionPassword.TrimStartAndEndInline();

    ActiveSessionSearch = MakeShared<FOnlineSessionSearch>();
    ActiveSessionSearch->MaxSearchResults = 100;
    ActiveSessionSearch->bIsLanQuery = bIsLanQuery;
    ActiveSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
        FOnFindSessionsCompleteDelegate::CreateUObject(this, &UBombTagGameInstance::HandleFindSessionsComplete));

    if (!SessionInterface->FindSessions(0, ActiveSessionSearch.ToSharedRef()))
    {
        UE_LOG(LogBombTag, Warning, TEXT("FindSessions failed to start."));
        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
        ActiveSessionSearch.Reset();
    }
}

void UBombTagGameInstance::StartHostedMatch()
{
    if (SessionInterface.IsValid())
    {
        if (!SessionInterface->StartSession(BombTagSessionName))
        {
            UE_LOG(LogBombTag, Verbose, TEXT("StartSession returned false for %s."), *BombTagSessionName.ToString());
        }
    }

    if (MatchMapName.IsNone())
    {
        UE_LOG(LogBombTag, Warning, TEXT("Match map name is not configured."));
        return;
    }

    if (UWorld* World = GetWorld())
    {
        if (World->GetNetMode() != NM_Client)
        {
            UGameplayStatics::OpenLevel(World, MatchMapName, true, TEXT("listen"));
        }
    }
}

void UBombTagGameInstance::LeaveSession()
{
    if (!SessionInterface.IsValid())
    {
        ReturnToMenuMap();
        return;
    }

    if (SessionInterface->GetNamedSession(BombTagSessionName) != nullptr)
    {
        bCreateSessionAfterDestroy = false;
        bReturnToMenuAfterDestroy = true;
        DestroySessionInternal();
    }
    else
    {
        ReturnToMenuMap();
    }
}

void UBombTagGameInstance::CreateSessionInternal()
{
    if (!SessionInterface.IsValid())
    {
        return;
    }

    PendingSessionSettings = MakeShared<FOnlineSessionSettings>();
    PendingSessionSettings->bIsLANMatch = bPendingHostLanMatch;
    PendingSessionSettings->bAllowJoinInProgress = true;
    PendingSessionSettings->bAllowJoinViaPresence = true;
    PendingSessionSettings->bShouldAdvertise = true;
    PendingSessionSettings->bUsesPresence = true;
    PendingSessionSettings->bUseLobbiesIfAvailable = true;
    PendingSessionSettings->NumPublicConnections = FMath::Clamp(PendingHostPublicConnections, 1, 64);
    PendingSessionSettings->Set(SETTING_MAPNAME, LobbyMapName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    PendingSessionSettings->Set(SessionDisplayNameKey, PendingHostSessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

    if (!PendingHostSessionPassword.IsEmpty())
    {
        PendingSessionSettings->Set(SessionPasswordKey, PendingHostSessionPassword, EOnlineDataAdvertisementType::DontAdvertise);
    }
    else
    {
        PendingSessionSettings->Remove(SessionPasswordKey);
    }

    CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
        FOnCreateSessionCompleteDelegate::CreateUObject(this, &UBombTagGameInstance::HandleCreateSessionComplete));

    if (!SessionInterface->CreateSession(0, BombTagSessionName, *PendingSessionSettings))
    {
        UE_LOG(LogBombTag, Warning, TEXT("CreateSession failed to start."));
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
        PendingSessionSettings.Reset();
    }
}

void UBombTagGameInstance::DestroySessionInternal()
{
    if (!SessionInterface.IsValid())
    {
        if (bReturnToMenuAfterDestroy)
        {
            ReturnToMenuMap();
        }
        return;
    }

    DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(
        FOnDestroySessionCompleteDelegate::CreateUObject(this, &UBombTagGameInstance::HandleDestroySessionComplete));

    if (!SessionInterface->DestroySession(BombTagSessionName))
    {
        UE_LOG(LogBombTag, Warning, TEXT("DestroySession failed to start."));
        SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
        HandleDestroySessionComplete(BombTagSessionName, false);
    }
}

void UBombTagGameInstance::HandleCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
    }

    PendingSessionSettings.Reset();
    bCreateSessionAfterDestroy = false;

    if (!bWasSuccessful)
    {
        UE_LOG(LogBombTag, Error, TEXT("Failed to create session %s."), *SessionName.ToString());
        return;
    }

    UE_LOG(LogBombTag, Log, TEXT("Session %s created successfully."), *SessionName.ToString());

    TravelToLobby();
}

void UBombTagGameInstance::HandleDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
    }

    if (!bWasSuccessful)
    {
        UE_LOG(LogBombTag, Warning, TEXT("Destroy session %s failed."), *SessionName.ToString());
    }

    if (bCreateSessionAfterDestroy)
    {
        bCreateSessionAfterDestroy = false;
        CreateSessionInternal();
        return;
    }

    if (bReturnToMenuAfterDestroy)
    {
        bReturnToMenuAfterDestroy = false;
        ReturnToMenuMap();
    }
}

void UBombTagGameInstance::HandleFindSessionsComplete(bool bWasSuccessful)
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
    }

    if (!ActiveSessionSearch.IsValid())
    {
        return;
    }

    if (!bWasSuccessful)
    {
        UE_LOG(LogBombTag, Warning, TEXT("FindSessions did not complete successfully."));
        ActiveSessionSearch.Reset();
        return;
    }

    const bool bHasNameFilter = !PendingJoinSessionName.IsEmpty();
    const FString DesiredPassword = PendingJoinSessionPassword;

    for (const FOnlineSessionSearchResult& Result : ActiveSessionSearch->SearchResults)
    {
        if (!Result.IsValid())
        {
            continue;
        }

        FString FoundName;
        Result.Session.SessionSettings.Get(SessionDisplayNameKey, FoundName);

        if (bHasNameFilter && !FoundName.Equals(PendingJoinSessionName, ESearchCase::IgnoreCase))
        {
            continue;
        }

        FString FoundPassword;
        Result.Session.SessionSettings.Get(SessionPasswordKey, FoundPassword);

        if (!DesiredPassword.IsEmpty() || !FoundPassword.IsEmpty())
        {
            if (!FoundPassword.Equals(DesiredPassword, ESearchCase::CaseSensitive))
            {
                continue;
            }
        }

        JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
            FOnJoinSessionCompleteDelegate::CreateUObject(this, &UBombTagGameInstance::HandleJoinSessionComplete));

        if (!SessionInterface->JoinSession(0, BombTagSessionName, Result))
        {
            UE_LOG(LogBombTag, Warning, TEXT("JoinSession failed to start."));
            SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
        }

        ActiveSessionSearch.Reset();
        return;
    }

    UE_LOG(LogBombTag, Warning, TEXT("No matching sessions were found."));
    ActiveSessionSearch.Reset();
}

void UBombTagGameInstance::HandleJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
    }

    if (Result != EOnJoinSessionCompleteResult::Success)
    {
        UE_LOG(LogBombTag, Warning, TEXT("Join session %s failed with result %d."), *SessionName.ToString(), static_cast<int32>(Result));
        return;
    }

    FString ConnectString;
    if (SessionInterface.IsValid() && SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
    {
        if (APlayerController* PlayerController = GetFirstLocalPlayerController())
        {
            PlayerController->ClientTravel(ConnectString, TRAVEL_Absolute);
        }
    }
    else
    {
        UE_LOG(LogBombTag, Warning, TEXT("Could not resolve connect string for session %s."), *SessionName.ToString());
    }
}

void UBombTagGameInstance::ReturnToMenuMap()
{
    if (MenuReturnURL.IsEmpty())
    {
        return;
    }

    if (UWorld* World = GetWorld())
    {
        if (World->GetNetMode() != NM_Client)
        {
            UGameplayStatics::OpenLevel(World, LobbyMapName, true);
        }
        else if (APlayerController* PlayerController = GetFirstLocalPlayerController())
        {
            PlayerController->ClientTravel(MenuReturnURL, TRAVEL_Absolute);
        }
    }
}

void UBombTagGameInstance::TravelToLobby()
{
    if (LobbyMapName.IsNone())
    {
        UE_LOG(LogBombTag, Warning, TEXT("Lobby map name is not configured."));
        return;
    }

    UGameplayStatics::OpenLevel(this, LobbyMapName, true, TEXT("listen"));
}

void UBombTagGameInstance::SetPlayerNickname(const FString& NewNickname)
{
    LoadOrCreatePlayerData();

    if (!PlayerSaveGame)
    {
        return;
    }

    FString SanitizedNickname = NewNickname;
    SanitizedNickname.TrimStartAndEndInline();

    if (!IsValidNickname(SanitizedNickname))
    {
        UE_LOG(LogBombTag, Warning, TEXT("Attempted to set invalid nickname '%s'."), *SanitizedNickname);
        return;
    }

    if (PlayerSaveGame->Nickname.Equals(SanitizedNickname, ESearchCase::CaseSensitive))
    {
        return;
    }

    PlayerSaveGame->Nickname = SanitizedNickname;
    SavePlayerData();
}

FString UBombTagGameInstance::GetPlayerNickname() const
{
    if (!PlayerSaveGame)
    {
        const_cast<UBombTagGameInstance*>(this)->LoadOrCreatePlayerData();
    }

    return PlayerSaveGame ? PlayerSaveGame->Nickname : FString();
}

void UBombTagGameInstance::RecordMatchResult(EBombTagMatchResult MatchResult)
{
    LoadOrCreatePlayerData();

    if (!PlayerSaveGame)
    {
        return;
    }

    switch (MatchResult)
    {
    case EBombTagMatchResult::Win:
        ++PlayerSaveGame->Win;
        break;
    case EBombTagMatchResult::Lose:
        ++PlayerSaveGame->Lose;
        break;
    default:
        break;
    }

    SavePlayerData();
}

void UBombTagGameInstance::GetPlayerRecord(int32& OutWin, int32& OutLose, int32& OutTotalMatches) const
{
    if (!PlayerSaveGame)
    {
        const_cast<UBombTagGameInstance*>(this)->LoadOrCreatePlayerData();
    }

    if (!PlayerSaveGame)
    {
        OutWin = 0;
        OutLose = 0;
        OutTotalMatches = 0;
        return;
    }

    OutWin = PlayerSaveGame->Win;
    OutLose = PlayerSaveGame->Lose;
    OutTotalMatches = OutWin + OutLose;
}

void UBombTagGameInstance::ResetPlayerRecord()
{
    LoadOrCreatePlayerData();

    if (!PlayerSaveGame)
    {
        return;
    }

    PlayerSaveGame->Win = 0;
    PlayerSaveGame->Lose = 0;

    SavePlayerData();
}

void UBombTagGameInstance::LoadOrCreatePlayerData()
{
    if (PlayerSaveGame)
    {
        return;
    }

    if (UGameplayStatics::DoesSaveGameExist(PlayerSaveSlotName, PlayerSaveSlotIndex))
    {
        USaveGame* LoadedSave = UGameplayStatics::LoadGameFromSlot(PlayerSaveSlotName, PlayerSaveSlotIndex);
        PlayerSaveGame = Cast<UBombTagSaveGame>(LoadedSave);

        if (!PlayerSaveGame)
        {
            UE_LOG(LogBombTag, Warning, TEXT("Existing save data could not be cast to UBombTagSaveGame. Creating new save."));
        }
    }

    if (!PlayerSaveGame)
    {
        PlayerSaveGame = Cast<UBombTagSaveGame>(UGameplayStatics::CreateSaveGameObject(UBombTagSaveGame::StaticClass()));

        if (!PlayerSaveGame)
        {
            UE_LOG(LogBombTag, Error, TEXT("Failed to create BombTag save game instance."));
            return;
        }

        SavePlayerData();
    }

    EnsureNicknameIsValid();
}

void UBombTagGameInstance::SavePlayerData()
{
    if (!PlayerSaveGame)
    {
        return;
    }

    if (!UGameplayStatics::SaveGameToSlot(PlayerSaveGame, PlayerSaveSlotName, PlayerSaveSlotIndex))
    {
        UE_LOG(LogBombTag, Warning, TEXT("Failed to save BombTag profile to slot %s"), PlayerSaveSlotName);
    }
}

void UBombTagGameInstance::EnsureNicknameIsValid()
{
    if (!PlayerSaveGame)
    {
        return;
    }

    FString SanitizedNickname = PlayerSaveGame->Nickname;
    SanitizedNickname.TrimStartAndEndInline();

    if (!SanitizedNickname.IsEmpty() && !IsValidNickname(SanitizedNickname))
    {
        UE_LOG(LogBombTag, Warning, TEXT("Loaded invalid nickname '%s'. Clearing saved value."), *SanitizedNickname);
        SanitizedNickname.Reset();
    }

    if (PlayerSaveGame->Nickname != SanitizedNickname)
    {
        PlayerSaveGame->Nickname = SanitizedNickname;
        SavePlayerData();
    }
}

bool UBombTagGameInstance::IsValidNickname(const FString& Nickname) const
{
    const int32 Length = Nickname.Len();

    if (Length < 4 || Length > 10)
    {
        return false;
    }

    for (const TCHAR Character : Nickname)
    {
        if (!IsAsciiAlphanumeric(Character))
        {
            return false;
        }
    }

    return true;
}

bool UBombTagGameInstance::IsAsciiAlphanumeric(TCHAR Character) const
{
    return (Character >= TEXT('0') && Character <= TEXT('9')) ||
        (Character >= TEXT('A') && Character <= TEXT('Z')) ||
        (Character >= TEXT('a') && Character <= TEXT('z'));
}