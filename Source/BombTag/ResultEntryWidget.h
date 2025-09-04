// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultEntryWidget.generated.h"

class UTextBlock;

UCLASS()
class BOMBTAG_API UResultEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock * FirstIDText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SecondIDText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ThirdIDText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* FourthIDText;
};
