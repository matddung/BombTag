#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UWidgetSwitcher;
class UWidget;
class UTextBlock;
class UEditableTextBox;
class UCheckBox;

UCLASS()
class BOMBTAG_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual bool Initialize() override;
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> MenuSwitcher;

    // Menu
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidget> MainMenu;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidget> MatchMenu;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidget> HostMenu;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidget> JoinMenu;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidget> MyRecordMenu;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidget> WaitingRoomMenu;

    // MenuButton
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> MatchButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> HostButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> JoinButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> MyRecordButton;

    // MatchMenu
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> MatchMenuTextBlock;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> MatchMenuBackButton;

    // HostMenu
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEditableTextBox> HostMenuTitleTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEditableTextBox> HostMenuPasswordTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCheckBox> HostMenuPasswordCheckBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> HostMenuCreateButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> HostMenuBackButton;

    // JoinMenu
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEditableTextBox> JoinMenuTitleTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEditableTextBox> JoinMenuPasswordTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> JoinMenuJoinButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> JoinMenuBackButton;

    // MyRecordMenu
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> MyRecordMenuBackButton;

    // WaitingRoomMenu
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> WaitingRoomMenuBackButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> WaitingRoomMenuStartButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> WaitingRoomMenuPlayer1Button;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> WaitingRoomMenuPlayer2Button;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> WaitingRoomMenuPlayer3Button;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> WaitingRoomMenuPlayer4Button;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> WaitingRoomMenuPlayer1Switcher;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> WaitingRoomMenuPlayer2Switcher;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> WaitingRoomMenuPlayer3Switcher;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> WaitingRoomMenuPlayer4Switcher;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> WaitingRoomMenuPlayer1IDTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> WaitingRoomMenuPlayer2IDTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> WaitingRoomMenuPlayer3IDTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> WaitingRoomMenuPlayer4IDTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> WaitingRoomMenuPlayer1RecordTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> WaitingRoomMenuPlayer2RecordTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> WaitingRoomMenuPlayer3RecordTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> WaitingRoomMenuPlayer4RecordTextBox;

    UFUNCTION()
    void OpenMatchMenu();

    UFUNCTION()
    void OpenHostMenu();

    UFUNCTION()
    void OpenJoinMenu();

    UFUNCTION()
    void OpenMyRecordMenu();

    UFUNCTION()
    void OpenMainMenu();

    UFUNCTION()
    void CreateHostMatch();

    UFUNCTION()
    void UpdateMatchMenuDots();

    UFUNCTION()
    void JoinMatch();

    UFUNCTION()
    void OnHostMenuPasswordCheckBoxChanged(bool bIsChecked);

    UFUNCTION()
    void WaitingRoomStart();

    UFUNCTION()
    void WaitingRoomPlayerMenu();

private:
    FTimerHandle MatchDotsTimerHandle;
    FTimerHandle HostWaitingDotsTimerHandle;
    int32 MatchDotCount = 1;
    int32 HostWaitingDotCount = 1;
};