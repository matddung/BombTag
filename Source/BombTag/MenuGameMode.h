#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameMode.generated.h"

UCLASS()
class BOMBTAG_API AMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMenuGameMode();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Menu")
	TSubclassOf<UUserWidget> MenuClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> Menu;
};