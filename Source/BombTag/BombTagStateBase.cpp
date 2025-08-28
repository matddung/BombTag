#include "BombTagStateBase.h"
#include "Net/UnrealNetwork.h"
#include "BombTagGameMode.h"

ABombTagStateBase::ABombTagStateBase()
{
    PrimaryActorTick.bCanEverTick = true;
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

void ABombTagStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABombTagStateBase, RemainingGameTime);
}