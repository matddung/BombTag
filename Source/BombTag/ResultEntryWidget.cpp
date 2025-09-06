#include "ResultEntryWidget.h"
#include "BombTagPlayerController.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UResultEntryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (APlayerController* PC = GetOwningPlayer())
    {
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputMode);
        PC->SetIgnoreMoveInput(true);
        PC->SetIgnoreLookInput(true);
        PC->bShowMouseCursor = true;

        if (ABombTagPlayerController* BTPC = Cast<ABombTagPlayerController>(PC))
        {
            BTPC->SetBorderFlashEnabled(false);
        }
    }

    if (UWorld* World = GetWorld())
    {
        if (AGameStateBase* GS = World->GetGameState<AGameStateBase>())
        {
            TArray<APlayerState*> PlayerStates = GS->PlayerArray;
            PlayerStates.Sort([](const APlayerState& A, const APlayerState& B)
                {
                    return A.GetScore() > B.GetScore();
                });

            const auto SetTextIfValid = [](UTextBlock* TextBlock, const FString& Name)
                {
                    if (TextBlock)
                    {
                        TextBlock->SetText(FText::FromString(Name));
                    }
                };

            if (PlayerStates.Num() > 0)
            {
                SetTextIfValid(FirstIDText, PlayerStates[0]->GetPlayerName());
            }
            if (PlayerStates.Num() > 1)
            {
                SetTextIfValid(SecondIDText, PlayerStates[1]->GetPlayerName());
            }
            if (PlayerStates.Num() > 2)
            {
                SetTextIfValid(ThirdIDText, PlayerStates[2]->GetPlayerName());
            }
            if (PlayerStates.Num() > 3)
            {
                SetTextIfValid(FourthIDText, PlayerStates[3]->GetPlayerName());
            }
        }
    }
}

void UResultEntryWidget::NativeDestruct()
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->SetIgnoreMoveInput(false);
        PC->SetIgnoreLookInput(false);
        PC->bShowMouseCursor = false;

        if (ABombTagPlayerController* BTPC = Cast<ABombTagPlayerController>(PC))
        {
            BTPC->SetBorderFlashEnabled(true);
        }
    }

    Super::NativeDestruct();
}

FReply UResultEntryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    OpenMenuMap();
    return FReply::Handled();
}

FReply UResultEntryWidget::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InTouchEvent)
{
    OpenMenuMap();
    return FReply::Handled();
}

void UResultEntryWidget::OpenMenuMap()
{
    if (UWorld* World = GetWorld())
    {
        const FString Options = TEXT("game=/Script/BombTag.MenuGameMode");
        UGameplayStatics::OpenLevel(World, FName(TEXT("/Game/Maps/MenuMap")), true, Options);
    }
}