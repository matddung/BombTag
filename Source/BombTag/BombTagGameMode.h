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

public:
	UFUNCTION(BlueprintPure, Category = "Game")
	float GetRemainingGameTime() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	float GameDuration = 10;

	FTimerHandle GameTimerHandle;
};