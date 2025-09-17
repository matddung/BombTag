#include "MainMenuWidget.h"
#include "BombTagGameInstance.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Widget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/CheckBox.h"
#include "Misc/Char.h"

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

    if (NewNicknameMenuConfirmButton)
    {
        NewNicknameMenuConfirmButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ConfirmNewNickname);
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

    bool bShouldShowNewNicknameMenu = false;

    if (UWorld* World = GetWorld())
    {
        if (UBombTagGameInstance* GameInstance = World->GetGameInstance<UBombTagGameInstance>())
        {
            FString SanitizedNickname = GameInstance->GetPlayerNickname();
            SanitizedNickname.TrimStartAndEndInline();
            bShouldShowNewNicknameMenu = SanitizedNickname.IsEmpty();
        }
    }

    if (bShouldShowNewNicknameMenu)
    {
        OpenNewNicknameMenu();
    }
    else
    {
        OpenMainMenu();
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
    UpdateMyRecordMenu();

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

void UMainMenuWidget::UpdateMyRecordMenu()
{
    UBombTagGameInstance* GameInstance = nullptr;

    if (UWorld* World = GetWorld())
    {
        GameInstance = World->GetGameInstance<UBombTagGameInstance>();
    }

    if (!GameInstance)
    {
        return;
    }

    const FString Nickname = GameInstance->GetPlayerNickname();
    if (MyRecordMenuNicknameText)
    {
        const FText NicknameText = Nickname.IsEmpty()
            ? NSLOCTEXT("MainMenu", "DefaultNickname", "Guest")
            : FText::FromString(Nickname);
        MyRecordMenuNicknameText->SetText(NicknameText);
    }

    int32 Win = 0;
    int32 Lose = 0;
    int32 TotalMatches = 0;
    GameInstance->GetPlayerRecord(Win, Lose, TotalMatches);

    if (MyRecordMenuWinText)
    {
        MyRecordMenuWinText->SetText(FText::FromString(FString::Printf(TEXT("Win : %d"), Win)));
    }

    if (MyRecordMenuLoseText)
    {
        MyRecordMenuLoseText->SetText(FText::FromString(FString::Printf(TEXT("Lose : %d"), Lose)));
    }

    if (MyRecordMenuRateText)
    {
        const float WinRate = TotalMatches > 0 ? (static_cast<float>(Win) / static_cast<float>(TotalMatches)) * 100.f : 0.f;
        MyRecordMenuRateText->SetText(FText::FromString(FString::Printf(TEXT("Rate : %.1f%%"), WinRate)));
    }
}

void UMainMenuWidget::OpenNewNicknameMenu()
{
    if (NewNicknameMenuNicknameTextBox)
    {
        UBombTagGameInstance* GameInstance = nullptr;
        if (UWorld* World = GetWorld())
        {
            GameInstance = World->GetGameInstance<UBombTagGameInstance>();
        }

        const FString CurrentNickname = GameInstance ? GameInstance->GetPlayerNickname() : FString();
        NewNicknameMenuNicknameTextBox->SetText(FText::FromString(CurrentNickname));
        UpdateNewNicknameError(CurrentNickname);
    }

    if (MenuSwitcher && NewNicknameMenu)
    {
        MenuSwitcher->SetActiveWidget(NewNicknameMenu);
    }
}

void UMainMenuWidget::ConfirmNewNickname()
{
    if (!NewNicknameMenuNicknameTextBox)
    {
        return;
    }

    FString EnteredNickname = NewNicknameMenuNicknameTextBox->GetText().ToString();
    EnteredNickname.TrimStartAndEndInline();

    UpdateNewNicknameError(EnteredNickname);

    if (!IsValidNickname(EnteredNickname))
    {
        return;
    }

    if (UWorld* World = GetWorld())
    {
        if (UBombTagGameInstance* GameInstance = World->GetGameInstance<UBombTagGameInstance>())
        {
            GameInstance->SetPlayerNickname(EnteredNickname);
        }
    }

    OpenMainMenu();
}

bool UMainMenuWidget::IsValidNickname(const FString& Nickname) const
{
    return GetNicknameValidationErrorText(Nickname).IsEmpty();
}

void UMainMenuWidget::OnNewNicknameTextChanged(const FText& NewText)
{
    UpdateNewNicknameError(NewText.ToString());
}

FText UMainMenuWidget::GetNicknameValidationErrorText(const FString& Nickname) const
{
    FString TrimmedNickname = Nickname;
    TrimmedNickname.TrimStartAndEndInline();

    if (TrimmedNickname.IsEmpty())
    {
        return NSLOCTEXT("MainMenu", "NicknameRequired", "Please enter your nickname");
    }

    const int32 Length = TrimmedNickname.Len();

    if (Length < 4 || Length > 10)
    {
        return NSLOCTEXT("MainMenu", "NicknameTooShort", "Nickname must be at least 4 characters and no more than 10 characters");
    }

    for (const TCHAR Character : TrimmedNickname)
    {
        if (!IsAsciiAlphanumeric(Character))
        {
            return NSLOCTEXT("MainMenu", "NicknameInvalidCharacters", "Only English letters and numbers can be used in your nickname");
        }
    }

    return FText::GetEmpty();
}

void UMainMenuWidget::UpdateNewNicknameError(const FString& Nickname)
{
    if (!NewNicknameMenuErrorText)
    {
        return;
    }

    const FText ValidationError = GetNicknameValidationErrorText(Nickname);

    NewNicknameMenuErrorText->SetText(ValidationError);

    if (!ValidationError.IsEmpty())
    {
        NewNicknameMenuErrorText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
    }
}

bool UMainMenuWidget::IsAsciiAlphanumeric(TCHAR Character) const
{
    return (Character >= TEXT('0') && Character <= TEXT('9')) ||
        (Character >= TEXT('A') && Character <= TEXT('Z')) ||
        (Character >= TEXT('a') && Character <= TEXT('z'));
}