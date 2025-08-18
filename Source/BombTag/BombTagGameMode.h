// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BombTagGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class ABombTagGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	ABombTagGameMode();
};



