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
    TObjectPtr<UWidget> HostWaitingMenu;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> MatchButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> HostButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> JoinButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> MyRecordButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> MatchMenuTextBlock;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> MatchMenuBackButton;

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

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEditableTextBox> JoinMenuTitleTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEditableTextBox> JoinMenuPasswordTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> JoinMenuJoinButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> JoinMenuBackButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> MyRecordMenuBackButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> HostWaitingMenuTextBlock;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> HostWaitingMenuStartButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> HostWaitingMenuBackButton;

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
    void CreateMatch();

    UFUNCTION()
    void UpdateMatchMenuDots();

    UFUNCTION()
    void JoinMatch();

    UFUNCTION()
    void OnHostMenuPasswordCheckBoxChanged(bool bIsChecked);

    UFUNCTION()
    void UpdateHostWaitingMenuDots();

    UFUNCTION()
    void HostMatchStart();

private:
    FTimerHandle MatchDotsTimerHandle;
    FTimerHandle HostWaitingDotsTimerHandle;
    int32 MatchDotCount = 1;
    int32 HostWaitingDotCount = 1;
};