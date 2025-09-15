#include "MenuGameMode.h"
#include "BombTagPlayerController.h"
#include "BombTagGameInstance.h"

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

void AMenuGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (MenuClass)
    {
        if (ABombTagPlayerController* BTPC = Cast<ABombTagPlayerController>(NewPlayer))
        {
            BTPC->ClientShowMainMenu(MenuClass);
        }
    }
}