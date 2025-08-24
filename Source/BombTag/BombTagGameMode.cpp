#include "BombTagGameMode.h"
#include "BombTagCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ABombTagGameMode::ABombTagGameMode()
{
	// stub
}

void ABombTagGameMode::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ABombTagGameMode::OnGameTimerExpired, GameDuration);

    TArray<AActor*> Characters;
    UGameplayStatics::GetAllActorsOfClass(this, ABombTagCharacter::StaticClass(), Characters);

    if (Characters.Num() > 0)
    {
        const int32 Index = FMath::RandRange(0, Characters.Num() - 1);
        if (ABombTagCharacter* Chosen = Cast<ABombTagCharacter>(Characters[Index]))
        {
            Chosen->SetHasBomb(true);
        }
    }
}

void ABombTagGameMode::OnGameTimerExpired()
{
    UE_LOG(LogTemp, Warning, TEXT("Game over: time limit reached"));
    UGameplayStatics::SetGamePaused(this, true);
}