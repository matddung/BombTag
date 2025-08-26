#include "BombTagPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "BombTag.h"
#include "UObject/ConstructorHelpers.h"
#include "Widgets/Input/SVirtualJoystick.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "BombTagGameMode.h"
#include "BombTagCharacter.h"

ABombTagPlayerController::ABombTagPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bTickEvenWhenPaused = true;
    static ConstructorHelpers::FClassFinder<UUserWidget> HUDBPClass(TEXT("/Game/UI/WBP_HUDWidget"));
    if (HUDBPClass.Succeeded())
    {
        HUDWidgetClass = HUDBPClass.Class;
    }
}

void ABombTagPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocalPlayerController() && HUDWidgetClass)
    {
        HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToPlayerScreen();
            TimerText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TimerText")));
            BorderFlash = Cast<UBorder>(HUDWidget->GetWidgetFromName(TEXT("BorderFlash")));
            if (BorderFlash)
            {
                BorderFlash->SetRenderOpacity(0.f);
            }
        }
    }

    if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
    {
        MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

        if (MobileControlsWidget)
        {
            MobileControlsWidget->AddToPlayerScreen(0);
        }
        else 
        {
            UE_LOG(LogBombTag, Error, TEXT("Could not spawn mobile controls widget."));
        }

    }
}

void ABombTagPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (IsLocalPlayerController())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
        {
            for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
            {
                Subsystem->AddMappingContext(CurrentContext, 0);
            }

            if (!SVirtualJoystick::ShouldDisplayTouchInterface())
            {
                for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
                {
                    Subsystem->AddMappingContext(CurrentContext, 0);
                }
            }
        }
    }
}

void ABombTagPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (TimerText)
    {
        if (ABombTagGameMode* GM = GetWorld()->GetAuthGameMode<ABombTagGameMode>())
        {
            const float Remaining = FMath::Max(0.f, GM->GetRemainingGameTime());
            TimerText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), Remaining)));
        }
    }

    if (BorderFlash)
    {
        ABombTagCharacter* Ch = Cast<ABombTagCharacter>(GetPawn());
        const bool bShouldFlash = Ch && Ch->HasBomb() && !GetWorld()->IsPaused();
        if (bShouldFlash)
        {
            BorderFlashElapsed += DeltaSeconds;
            const float Alpha = 0.5f * (1.f - FMath::Cos(2.f * PI * BorderFlashElapsed));
            BorderFlash->SetRenderOpacity(Alpha);
        }
        else
        {
            BorderFlashElapsed = 0.f;
            BorderFlash->SetRenderOpacity(0.f);
        }
    }
}