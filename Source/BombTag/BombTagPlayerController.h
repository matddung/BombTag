#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BombTagPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;
class UTextBlock;
class UBorder;

UCLASS(abstract)
class ABombTagPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ABombTagPlayerController();

protected:
    UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
    TArray<UInputMappingContext*> DefaultMappingContexts;

    UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
    TArray<UInputMappingContext*> MobileExcludedMappingContexts;

    UPROPERTY(EditAnywhere, Category = "Input|Touch Controls")
    TSubclassOf<UUserWidget> MobileControlsWidgetClass;

    TObjectPtr<UUserWidget> MobileControlsWidget;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> HUDWidgetClass;

    TObjectPtr<UUserWidget> HUDWidget;

    TObjectPtr<UTextBlock> TimerText;

    TObjectPtr<UBorder> BorderFlash;

    float BorderFlashElapsed = 0.f;

    virtual void BeginPlay() override;

    virtual void SetupInputComponent() override;

    virtual void Tick(float DeltaSeconds) override;

};