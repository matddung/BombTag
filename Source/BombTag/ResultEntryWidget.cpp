#include "ResultEntryWidget.h"

#include "GameFramework/PlayerController.h"
#include "BombTagPlayerController.h"

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