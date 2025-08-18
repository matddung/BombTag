#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UWidgetSwitcher;
class UWidget;

UCLASS()
class BOMBTAG_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual bool Initialize() override;

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
    TObjectPtr<UButton> MatchButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> HostButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> JoinButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> MyRecordButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> MatchMenuBackButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> HostMenuBackButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> JoinMenuBackButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> MyRecordMenuBackButton;

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
};