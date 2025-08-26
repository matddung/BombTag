#include "BombTagGameMode.h"
#include "BombTagCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

ABombTagGameMode::ABombTagGameMode()
{
	
}

void ABombTagGameMode::BeginPlay()
{
    Super::BeginPlay();

    StartNewRound();
}

void ABombTagGameMode::OnGameTimerExpired()
{
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(this, ABombTagCharacter::StaticClass(), Actors);

    TArray<ABombTagCharacter*> Characters;
    for (AActor* A : Actors)
    {
        if (ABombTagCharacter* Ch = Cast<ABombTagCharacter>(A))
        {
            Characters.Add(Ch);
        }
    }

    if (Characters.Num() <= 2)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("BOOM!"));
        }

        for (ABombTagCharacter* Ch : Characters)
        {
            const bool bWinner = !Ch->HasBomb();
            if (APlayerController* PC = Cast<APlayerController>(Ch->GetController()))
            {
                PC->ClientMessage(bWinner ? TEXT("WIN") : TEXT("Lose"));
            }
        }

        UGameplayStatics::SetGamePaused(this, true);
        return;
    }

    ABombTagCharacter* BombHolder = nullptr;
    for (ABombTagCharacter* Ch : Characters)
    {
        if (Ch->HasBomb())
        {
            BombHolder = Ch;
            break;
        }
    }

    if (BombHolder)
    {
        BombHolder->Destroy();
    }

    StartNewRound();
}

float ABombTagGameMode::GetRemainingGameTime() const
{
    return GetWorldTimerManager().GetTimerRemaining(GameTimerHandle);
}

void ABombTagGameMode::StartNewRound()
{
    GetWorldTimerManager().ClearTimer(GameTimerHandle);
    GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ABombTagGameMode::OnGameTimerExpired, GameDuration);

    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(this, ABombTagCharacter::StaticClass(), Actors);

    TArray<ABombTagCharacter*> Characters;
    for (AActor* A : Actors)
    {
        if (ABombTagCharacter* Ch = Cast<ABombTagCharacter>(A))
        {
            Ch->SetHasBomb(false);
            Characters.Add(Ch);
        }
    }

    if (Characters.Num() > 0)
    {
        const int32 Index = FMath::RandRange(0, Characters.Num() - 1);
        Characters[Index]->SetHasBomb(true);
    }
}