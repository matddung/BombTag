#include "BombTagGameInstance.h"
#include "BombTag.h"
#include "BombTagSaveGame.h"

#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"

namespace
{
    constexpr int32 PlayerSaveSlotIndex = 0;
    const TCHAR* PlayerSaveSlotName = TEXT("PlayerProfile");
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