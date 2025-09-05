#include "BombTagStateBase.h"
#include "Net/UnrealNetwork.h"
#include "BombTagGameMode.h"

ABombTagStateBase::ABombTagStateBase()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bTickEvenWhenPaused = true;
    bReplicates = true;
}

void ABombTagStateBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (HasAuthority())
    {
        if (ABombTagGameMode* GM = GetWorld()->GetAuthGameMode<ABombTagGameMode>())
        {
            RemainingGameTime = GM->GetRemainingGameTime();
        }
    }
}

float ABombTagStateBase::GetRemainingGameTime() const
{
    if (HasAuthority())
    {
        if (const ABombTagGameMode* GM = GetWorld()->GetAuthGameMode<ABombTagGameMode>())
        {
            return GM->GetRemainingGameTime();
        }
    }

    return RemainingGameTime;
}

void ABombTagStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABombTagStateBase, RemainingGameTime);
}