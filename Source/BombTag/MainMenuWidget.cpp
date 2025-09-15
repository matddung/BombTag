#include "MainMenuWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Widget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/CheckBox.h"

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

    if (HostMenuPasswordCheckBox)
    {
        HostMenuPasswordCheckBox->OnCheckStateChanged.AddDynamic(this, &UMainMenuWidget::OnHostMenuPasswordCheckBoxChanged);
    }

    if (HostMenuCreateButton)
    {
        HostMenuCreateButton->OnClicked.AddDynamic(this, &UMainMenuWidget::CreateHostMatch);
    }

    if (HostMenuBackButton)
    {
        HostMenuBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenMainMenu);
    }

    if (JoinMenuBackButton)
    {
        JoinMenuBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenMainMenu);
    }

    if (WaitingRoomMenuStartButton)
    {
        WaitingRoomMenuStartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::WaitingRoomStart);
    }

    if (WaitingRoomMenuBackButton)
    {
        WaitingRoomMenuBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenMainMenu);
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

    GetWorld()->GetTimerManager().SetTimer(MatchDotsTimerHandle, this, &UMainMenuWidget::UpdateMatchMenuDots, 0.4f, true);

    if (HostMenuPasswordCheckBox)
    {
        OnHostMenuPasswordCheckBoxChanged(HostMenuPasswordCheckBox->IsChecked());
    }
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

void UMainMenuWidget::CreateHostMatch()
{
    if (MenuSwitcher && WaitingRoomMenu)
    {
        MenuSwitcher->SetActiveWidget(WaitingRoomMenu);
    }
}

void UMainMenuWidget::UpdateMatchMenuDots()
{
    MatchDotCount = (MatchDotCount % 3) + 1;
    const FString Dots = FString::ChrN(MatchDotCount, TEXT('.'));
    const FText Base = NSLOCTEXT("Match", "Searching", "Searching for Match");
    const FText Final = FText::FromString(Base.ToString() + Dots);
    if (MatchMenuTextBlock) { MatchMenuTextBlock->SetText(Final); }
}

void UMainMenuWidget::JoinMatch()
{

}

void UMainMenuWidget::OnHostMenuPasswordCheckBoxChanged(bool bIsChecked)
{
    if (HostMenuPasswordTextBox)
    {
        HostMenuPasswordTextBox->SetIsEnabled(bIsChecked);
    }
}

void UMainMenuWidget::WaitingRoomStart()
{

}

void UMainMenuWidget::WaitingRoomPlayerMenu(int32 PlayerIndex)
{
    UWidgetSwitcher* TargetSwitcher = nullptr;

    switch (PlayerIndex)
    {
    case 1:
        TargetSwitcher = WaitingRoomMenuPlayer1Switcher;
        break;
    case 2:
        TargetSwitcher = WaitingRoomMenuPlayer2Switcher;
        break;
    case 3:
        TargetSwitcher = WaitingRoomMenuPlayer3Switcher;
        break;
    case 4:
        TargetSwitcher = WaitingRoomMenuPlayer4Switcher;
        break;
    default:
        break;
    }

    if (TargetSwitcher)
    {
        TargetSwitcher->SetActiveWidgetIndex(1);
    }
}