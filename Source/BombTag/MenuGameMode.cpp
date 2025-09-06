#include "MenuGameMode.h"
#include "BombTagPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

AMenuGameMode::AMenuGameMode()
{
    DefaultPawnClass = nullptr;

    static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/UI/WBP_MainMenu"));
    if (MenuBPClass.Succeeded())
    {
        MenuClass = MenuBPClass.Class;
    }
}

void AMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC && MenuClass)
    {
        Menu = CreateWidget<UUserWidget>(PC, MenuClass);
        if (Menu)
        {
            Menu->AddToViewport();

            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(Menu->TakeWidget());
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }
    }

    if (ABombTagPlayerController* BTPC = Cast<ABombTagPlayerController>(PC))
    {
        BTPC->ShowHUDWidget();
    }
}
