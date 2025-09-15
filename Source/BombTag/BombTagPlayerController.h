#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BombTagPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;
class UTextBlock;
class UBorder;
class UResultEntryWidget;

UCLASS(abstract)
class BOMBTAG_API ABombTagPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ABombTagPlayerController();

    void SetBorderFlashEnabled(bool bEnabled);

    void ShowHUDWidget();

protected:
    UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
    TArray<UInputMappingContext*> DefaultMappingContexts;

    UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
    TArray<UInputMappingContext*> MobileExcludedMappingContexts;

    UPROPERTY(EditAnywhere, Category = "Input|Touch Controls")
    TSubclassOf<UUserWidget> MobileControlsWidgetClass;

    UPROPERTY()
    TObjectPtr<UUserWidget> MobileControlsWidget;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> HUDWidgetClass;

    UPROPERTY()
    TObjectPtr<UUserWidget> HUDWidget;

    UPROPERTY()
    TObjectPtr<UUserWidget> MenuWidget;

    UPROPERTY()
    TObjectPtr<UTextBlock> TimerText;

    UPROPERTY()
    TObjectPtr<UBorder> BorderFlash;

    float BorderFlashElapsed = 0.f;
    bool bBorderFlashEnabled = true;

    virtual void BeginPlay() override;

    virtual void SetupInputComponent() override;

    virtual void Tick(float DeltaSeconds) override;

public:
    UFUNCTION(Client, Reliable)
    void ClientShowResultScreen(TSubclassOf<UResultEntryWidget> ResultWidgetClass, bool bWinner);

    UFUNCTION(Client, Reliable)
    void ClientShowMainMenu(TSubclassOf<UUserWidget> InMenuClass);
};