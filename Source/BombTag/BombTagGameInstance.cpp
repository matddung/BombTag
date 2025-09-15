#include "BombTagGameInstance.h"
#include "BombTag.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"

void UBombTagGameInstance::Init()
{
    Super::Init();

    UE_LOG(LogTemp, Warning, TEXT("My Custom GameInstance Initialized!"));

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