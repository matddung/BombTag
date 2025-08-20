#include "MainMenuWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Widget.h"
#include "Components/TextBlock.h"

bool UMainMenuWidget::Initialize()
{
    const bool bSuccess = Super::Initialize();

    if (!bSuccess)
    {
        return false;
    }

    if (MatchButton)
    {
        MatchButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenMatchMenu);
    }

    if (HostButton)
    {
        HostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenHostMenu);
    }

    if (JoinButton)
    {
        JoinButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenJoinMenu);
    }

    if (MyRecordButton)
    {
        MyRecordButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenMyRecordMenu);
    }

    if (MatchMenuBackButton)
    {
        MatchMenuBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenMainMenu);
    }

    if (HostMenuCreateButton)
    {
        HostMenuCreateButton->OnClicked.AddDynamic(this, &UMainMenuWidget::CreateMatch);
    }

    if (HostMenuBackButton)
    {
        HostMenuBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenMainMenu);
    }

    if (JoinMenuBackButton)
    {
        JoinMenuBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenMainMenu);
    }

    if (MyRecordMenuBackButton)
    {
        MyRecordMenuBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenMainMenu);
    }

    return true;
}

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GetWorld()->GetTimerManager().SetTimer(
        DotsTimerHandle, this, &UMainMenuWidget::UpdateDots, 0.4f, true
    );
}

void UMainMenuWidget::OpenMatchMenu()
{
    if (MenuSwitcher && MatchMenu)
    {
        MenuSwitcher->SetActiveWidget(MatchMenu);
    }
}

void UMainMenuWidget::OpenHostMenu()
{
    if (MenuSwitcher && HostMenu)
    {
        MenuSwitcher->SetActiveWidget(HostMenu);
    }
}

void UMainMenuWidget::OpenJoinMenu()
{
    if (MenuSwitcher && JoinMenu)
    {
        MenuSwitcher->SetActiveWidget(JoinMenu);
    }
}

void UMainMenuWidget::OpenMyRecordMenu()
{
    if (MenuSwitcher && MyRecordMenu)
    {
        MenuSwitcher->SetActiveWidget(MyRecordMenu);
    }
}

void UMainMenuWidget::OpenMainMenu()
{
    if (MenuSwitcher && MainMenu)
    {
        MenuSwitcher->SetActiveWidget(MainMenu);
    }
}

void UMainMenuWidget::CreateMatch()
{

}

void UMainMenuWidget::UpdateDots()
{
    DotCount = (DotCount % 3) + 1;
    const FString Dots = FString::ChrN(DotCount, TEXT('.'));
    const FText Base = NSLOCTEXT("Match", "Searching", "Searching for Match");
    const FText Final = FText::FromString(Base.ToString() + Dots);
    if (MatchMenuTextBlock) { MatchMenuTextBlock->SetText(Final); }
}

void UMainMenuWidget::JoinMatch()
{

}