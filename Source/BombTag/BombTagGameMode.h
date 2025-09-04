#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BombTagGameMode.generated.h"

UCLASS(abstract)
class ABombTagGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ABombTagGameMode();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnGameTimerExpired();

	void StartNewRound();

	void HandleStartCountdown();

	void RespawnPlayers();

	void BeginStartCountdown();

public:
	UFUNCTION(BlueprintPure, Category = "Game")
	float GetRemainingGameTime() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	float GameDuration = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	float StartDelay = 3.f;

	FTimerHandle GameTimerHandle;
	FTimerHandle CountdownTimerHandle;

	int32 CountdownTime;
};