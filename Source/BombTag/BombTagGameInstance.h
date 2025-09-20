#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "BombTagGameInstance.generated.h"

class UBombTagSaveGame;

UENUM(BlueprintType)
enum class EBombTagMatchResult : uint8
{
    Win,
    Lose
};

UCLASS()
class BOMBTAG_API UBombTagGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;

    UFUNCTION(BlueprintCallable, Category = "Player Profile")
    void SetPlayerNickname(const FString& NewNickname);

    UFUNCTION(BlueprintPure, Category = "Player Profile")
    FString GetPlayerNickname() const;

    UFUNCTION(BlueprintCallable, Category = "Player Profile")
    void RecordMatchResult(EBombTagMatchResult MatchResult);

    UFUNCTION(BlueprintPure, Category = "Player Profile")
    void GetPlayerRecord(int32& OutWin, int32& OutLose, int32& OutTotalMatches) const;

    UFUNCTION(BlueprintCallable, Category = "Player Profile")
    void ResetPlayerRecord();

    UFUNCTION(BlueprintCallable, Category = "Online|Sessions")
    void HostOnlineSession(const FString& SessionName, const FString& SessionPassword, int32 MaxPublicConnections, bool bIsLanMatch);

    UFUNCTION(BlueprintCallable, Category = "Online|Sessions")
    void FindAndJoinSession(const FString& SessionName, const FString& SessionPassword, bool bIsLanQuery);

    UFUNCTION(BlueprintCallable, Category = "Online|Sessions")
    void StartHostedMatch();

    UFUNCTION(BlueprintCallable, Category = "Online|Sessions")
    void LeaveSession();

private:
    void AttemptDeviceIdLogin();
    bool bAttemptedDeviceIdLogin = false;

    void HandleLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

    void CreateSessionInternal();
    void DestroySessionInternal();
    void HandleCreateSessionComplete(FName SessionName, bool bWasSuccessful);
    void HandleDestroySessionComplete(FName SessionName, bool bWasSuccessful);
    void HandleFindSessionsComplete(bool bWasSuccessful);
    void HandleJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
    void ReturnToMenuMap();
    void TravelToLobby();

    void LoadOrCreatePlayerData();
    void SavePlayerData();
    void EnsureNicknameIsValid();
    bool IsValidNickname(const FString& Nickname) const;
    bool IsAsciiAlphanumeric(TCHAR Character) const;

    IOnlineIdentityPtr IdentityInterface;
    IOnlineSessionPtr SessionInterface;
    FDelegateHandle LoginCompleteDelegateHandle;
    FDelegateHandle CreateSessionCompleteDelegateHandle;
    FDelegateHandle DestroySessionCompleteDelegateHandle;
    FDelegateHandle FindSessionsCompleteDelegateHandle;
    FDelegateHandle JoinSessionCompleteDelegateHandle;

    TSharedPtr<class FOnlineSessionSettings> PendingSessionSettings;
    TSharedPtr<class FOnlineSessionSearch> ActiveSessionSearch;

    bool bCreateSessionAfterDestroy = false;
    bool bReturnToMenuAfterDestroy = false;

    FString PendingHostSessionName;
    FString PendingHostSessionPassword;
    int32 PendingHostPublicConnections = 4;
    bool bPendingHostLanMatch = false;

    FString PendingJoinSessionName;
    FString PendingJoinSessionPassword;
    UPROPERTY(EditDefaultsOnly, Category = "Online|Sessions")
    FName LobbyMapName = FName(TEXT("/Game/Maps/MenuMap"));

    UPROPERTY(EditDefaultsOnly, Category = "Online|Sessions")
    FName MatchMapName = FName(TEXT("/Game/Maps/MainMap"));

    UPROPERTY(EditDefaultsOnly, Category = "Online|Sessions")
    FString MenuReturnURL = TEXT("/Game/Maps/MenuMap");

    UPROPERTY()
    UBombTagSaveGame* PlayerSaveGame = nullptr;
};