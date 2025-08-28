#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BombTagStateBase.generated.h"

UCLASS()
class BOMBTAG_API ABombTagStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ABombTagStateBase();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, Category = "Game")
	float GetRemainingGameTime() const { return RemainingGameTime; }

protected:
	UPROPERTY(Replicated)
	float RemainingGameTime = 0.f;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};