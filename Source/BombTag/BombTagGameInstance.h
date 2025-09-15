// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "BombTagGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BOMBTAG_API UBombTagGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

private:
	void AttemptDeviceIdLogin();
    bool bAttemptedDeviceIdLogin = false;

    void HandleLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

    IOnlineIdentityPtr IdentityInterface;
    FDelegateHandle LoginCompleteDelegateHandle;
};